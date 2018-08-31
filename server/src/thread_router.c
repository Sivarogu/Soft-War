#include "thread_router.h"
#include "command.h"
#include "game.h"
#include "network.h"
#include "log.h"
#include "global_config.h"

int thread_router_start() {
    t_command *message;

    zsock_t *router = zsock_new(ZMQ_ROUTER);
    zsock_bind(router, "tcp://*:%zu", global_config.rep_port);
    zsock_set_rcvtimeo(router, global_config.cycle_interval);
    log_debug("router socket bound on port %zu", global_config.rep_port);
    while (!zsys_interrupted) {
        log_debug("game is pending");
        pthread_mutex_lock(&game_info_mutex);
        game_info.game_status = GAME_STATUS_PENDING;
        while (game_info.energy_cells)
            energy_cell_destroy(game_info.energy_cells);
        pthread_mutex_unlock(&game_info_mutex);
        while (!zsys_interrupted && get_nb_player() < 4)
        {
            message = request_receive(router);
            if (message != NULL)
                command_handle_lobby(router, message);
        }
        pthread_mutex_lock(&game_info_mutex);
        pthread_cond_signal(&game_info_mutex_start_cond);
        pthread_mutex_unlock(&game_info_mutex);
        while (!zsys_interrupted && game_info.players && game_info.players->next)
        {
            message = request_receive(router);
            if (message != NULL)
                command_handle_game(router, message);
        }
        pthread_mutex_lock(&game_info_mutex);
        game_info.game_status = GAME_STATUS_FINISHED;
        pthread_cond_wait(&game_info_mutex_start_cond, &game_info_mutex);
        pthread_mutex_unlock(&game_info_mutex);
    }
    zsock_destroy(&router);
    BIND_NEG(log_debug("router socket destroyed"));
    return 0;
}

void *thread_router_run() {
	if (thread_router_start() < 0) {
		log_error("the router thread failed. aborting...");
	}
	pthread_exit(NULL);
}
