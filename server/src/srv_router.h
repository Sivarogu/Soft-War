#pragma once
#include "librairies.h"
#include "structs.h"
#include "command_hub.h"

void start_router();
t_command *cmd_recv(zsock_t *router);
void send_response(zsock_t *socket, zframe_t *identity, char *result, char *data);
