#pragma once
#include "librairies.h"
#include "structs.h"
#include "srv_router.h"
#include "commands.h"

void handle_cmd(zsock_t *router, t_command *command);
void destroy_command(t_command *command);
