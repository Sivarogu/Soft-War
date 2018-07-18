#include "srv_publish.h"

void *start_publish(void *srv_game_info) {
    char message[36];

    t_game_info *game_info = (t_game_info *)(srv_game_info);
    zsock_t *publish = zsock_new(ZMQ_PUB);
    zsock_bind(publish, "tcp://*:4243");

    pthread_mutex_lock(&game_info->mutex_game);
    pthread_cond_wait(&game_info->mutex_start_cond, &game_info->mutex_game);
    sprintf(message, "{\"notification_type\":%d,\"data\":null}", GAME_STARTED);
    if (zstr_send(publish, message) != 0)
        printf("Error sending pub message : %s", message);
    char *cycle_info = json_encode_cycle(game_info);
    if (zstr_send(publish, cycle_info) != 0)
        printf("Error sending pub message : %s", message);
    free(cycle_info);
    pthread_mutex_unlock(&game_info->mutex_game);
    zclock_sleep(5000);
    while (!zsys_interrupted && game_info->game_status != ENDED)
    {
        pthread_mutex_lock(&game_info->mutex_game);
        new_game_cycle(game_info);
        handle_dead_players(game_info, publish);
        char *cycle_info = json_encode_cycle(game_info);
        pthread_mutex_unlock(&game_info->mutex_game);
        if (zstr_send(publish, cycle_info) != 0)
        {
            printf("Error sending pub message : %s", message);
            break;
        }
        printf("Message envoyé : %s\n", message);
        free(cycle_info);
        zclock_sleep(5000);
    }
    if (game_info->first_player) {
        sprintf(message, "{\"notification_type\":%d,\"data\":null}", CLIENT_WIN);
        if (zstr_send(publish, message) != 0)
            printf("Error sending pub message : %s", message);
    }
    sprintf(message, "{\"notification_type\":%d,\"data\":null}", GAME_FINISHED);
    if (zstr_send(publish, message) != 0)
        printf("Error sending pub message : %s", message);
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
            sprintf(message, "{\"notification_type\":%d,\"data\":null}", CLIENT_DEAD);
            if (zstr_send(publish, message) != 0)
                printf("Error sending pub message : %s", message);
            destroy_player(player, game_info);
        }
        player = temp_pl;
    }
}
