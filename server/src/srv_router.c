#include "srv_router.h"

void *start_router(void *srv_game_info)
{
    t_command *message;

    t_game_info *game_info = (t_game_info *)(srv_game_info);
    zsock_t *router = zsock_new(ZMQ_ROUTER);
    zsock_bind(router, "tcp://*:4242");
    zsock_set_rcvtimeo(router, 500);
    while (!zsys_interrupted && get_nb_player(game_info) < 4)
    // while (!zsys_interrupted && get_nb_player(game_info) < 1)
    {
        message = cmd_recv(router);
        if (message != NULL)
            handle_lobby_cmd(game_info, router, message);
    }
    pthread_mutex_lock(&game_info->mutex_game);
    game_info->game_status = ACTIVE;
    pthread_cond_signal(&game_info->mutex_start_cond);
    pthread_mutex_unlock(&game_info->mutex_game);
    while (!zsys_interrupted
        // && game_info->first_player)
        && game_info->first_player
        && game_info->first_player->next)
    {
        message = cmd_recv(router);
        if (message != NULL)
            handle_game_cmd(game_info, router, message);
    }
    pthread_mutex_lock(&game_info->mutex_game);
    game_info->game_status = ENDED;
    pthread_mutex_unlock(&game_info->mutex_game);
    zsock_destroy(&router);
    pthread_exit(NULL);
}

t_command *cmd_recv(zsock_t *router)
{
    char delimiter[2] = "|";
    zmsg_t *rcv_data;
    zframe_t *empty;
    zframe_t *zmessage;
    char *message;
    t_command *command;

    rcv_data = zmsg_recv(router);
    if (rcv_data == NULL)
        return NULL;
    command = malloc(sizeof(t_command));
    command->identity = zmsg_pop(rcv_data);
    empty = zmsg_pop(rcv_data);
    zmessage = zmsg_pop(rcv_data);
    zmsg_destroy(&rcv_data);
    message = zframe_strdup(zmessage);
    command->name = strtok_r(message, delimiter, &message);
    command->params = strtok_r(NULL, delimiter, &message);
    zframe_destroy(&empty);
    zframe_destroy(&zmessage);
    return command;
}

void send_response(zsock_t *socket, zframe_t *identity, char *result, char *data)
{
    zframe_t *empty;
    zframe_t *zcontent;
    char *content;
    zmsg_t *response;

    empty = zframe_new_empty();
    content = malloc((strlen(result) + strlen(data) + 2) * sizeof(char));
    sprintf(content, "%s|%s", result, data);
    zcontent = zframe_from(content);
    response = zmsg_new();
    zmsg_prepend(response, &identity);
    zmsg_append(response, &empty);
    zmsg_append(response, &zcontent);
    zmsg_send(&response, socket);
    zmsg_destroy(&response);
    zframe_destroy(&identity);
    zframe_destroy(&empty);
    zframe_destroy(&zcontent);
    free(content);
}
