#include "srv_router.h"

void start_router()
{
    zsock_t *router = zsock_new(ZMQ_ROUTER);
    t_command *message;

    zsock_bind(router, "tcp://*:8765");
    while (!zsys_interrupted)
    {
        message = cmd_recv(router);
        handle_cmd(router, message);
    }
    zsock_destroy(&router);
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
    command = malloc(sizeof(t_command));
    command->identity = zmsg_pop(rcv_data);
    empty = zmsg_pop(rcv_data);
    zmessage = zmsg_pop(rcv_data);
    zmsg_destroy(&rcv_data);
    message = zframe_strdup(zmessage);
    command->name = strtok(message, delimiter);
    command->params = strtok(NULL, delimiter);
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
    strcpy(content, result);
    strcat(content, "|");
    strcat(content, data);
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
