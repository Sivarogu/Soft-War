#include "thread_publish.h"
#include <czmq.h>
#include "global_config.h"
#include "utils.h"
#include "network.h"
#include "game.h"

static int handle_dead_players(zsock_t *socket) {
    t_player *next_player;

    for (t_player *player = game_info.players; player; player = next_player) {
        next_player = player->next;
        if (player->energy <= 0 || player->energy > 100) {
            BIND_NEG(publish(socket, NOTIFICATION_TYPE_CLIENT_DEAD, json_null(), player->id));
            player_destroy(player);
        }
    }
	return 0;
}

int thread_publish_start() {
    zsock_t *socket;

	BIND_ZERO((socket = zsock_new(ZMQ_PUB)));
    zsock_bind(socket, "tcp://*:%zu", global_config.pub_port);
	BIND_NEG(log_debug("publish socket bound"));

    while (!zsys_interrupted) {
        pthread_mutex_lock(&game_info_mutex);
        pthread_cond_wait(&game_info_mutex_start_cond, &game_info_mutex);

        BIND_NEG(publish(socket, NOTIFICATION_TYPE_GAME_STARTED, json_null(), CHANNEL_GLOBAL));
        BIND_NEG(log_info("game started"));

        BIND_NEG(publish_cycle(socket));
        pthread_mutex_unlock(&game_info_mutex);
        zclock_sleep(global_config.cycle_interval);

        BIND_NEG(thread_publish_start_cycles(socket));
        pthread_mutex_lock(&game_info_mutex);
        pthread_cond_signal(&game_info_mutex_start_cond);
        pthread_mutex_unlock(&game_info_mutex);
    }
    zsock_destroy(&socket);
    BIND_NEG(log_debug("publish socket destroyed"));
	return 0;
}

int thread_publish_start_cycles(zsock_t *socket) {
    while (!zsys_interrupted && game_info.game_status != GAME_STATUS_FINISHED) {
        pthread_mutex_lock(&game_info_mutex);
        BIND_NEG(next_game_cycle());
        BIND_NEG(handle_dead_players(socket));
        BIND_NEG(publish_cycle(socket));
        pthread_mutex_unlock(&game_info_mutex);
        zclock_sleep(global_config.cycle_interval);
    }
    if (game_info.players) {
		BIND_NEG(publish(socket, NOTIFICATION_TYPE_CLIENT_WIN, json_null(), game_info.players->id));
        pthread_mutex_lock(&game_info_mutex);
        while (game_info.players)
            player_destroy(game_info.players);
        pthread_mutex_unlock(&game_info_mutex);
    }
	BIND_NEG(publish(socket, NOTIFICATION_TYPE_GAME_FINISHED, json_null(), CHANNEL_GLOBAL));
	return 0;
}

void *thread_publish_run() {
	if (thread_publish_start() < 0)
		log_error("the publish thread failed. aborting...");
	pthread_exit(NULL);
}
