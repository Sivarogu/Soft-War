#include "srv_publish.h"

void *start_publish(void *srv_game_info) {
    char message[255];

    t_game_info *game_info = (t_game_info *)(srv_game_info);
    zsock_t *publish = zsock_new(ZMQ_PUB);
    zsock_bind(publish, "tcp://*:4243");
    while (!zsys_interrupted && game_info->game_status != -1)
    {
        zclock_sleep(5000);
        pthread_mutex_lock(&game_info->mutex_game);
        new_game_cycle(game_info);
        handle_dead_players(game_info, publish);
        sprintf(message, "# Game status : %d. Message : Tout fonctionne", game_info->game_status);
        pthread_mutex_unlock(&game_info->mutex_game);
        if (zstr_send(publish, message) != 0) {
            printf("Error in sending pub message");
            break;
        }
        printf("Message envoyé : %s\n", message);
    }
    zsock_destroy(&publish);
    pthread_exit(NULL);
}

void handle_dead_players(t_game_info *game_info, zsock_t *publish) {
    t_player *temp_pl;

    t_player *player = game_info->first_player;
    while (player)
    {
        temp_pl = player->next;
        if (player->energy <= 0 || player->energy > 100) {
            char message[255];
            sprintf(message, "# Notif player %s : you're dead !", player->id);
            if (zstr_send(publish, message) != 0)
                printf("Error sending pub message : %s", message);
            printf("Message envoyé : %s\n", message);
            destroy_player(player, game_info);
        }
        player = temp_pl;
    }
}
