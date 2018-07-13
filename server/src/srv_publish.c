#include "srv_publish.h"

void *start_publish(void *srv_game_info) {
    t_game_info *game_info = (t_game_info *)(srv_game_info);
    zsock_t *publish = zsock_new(ZMQ_PUB);
    zsock_bind(publish, "tcp://*:4243");
    while (!zsys_interrupted && game_info->game_status != -1)
    {
        zclock_sleep(5000);
        char message[255];
        pthread_mutex_lock(&game_info->mutex_game);
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
