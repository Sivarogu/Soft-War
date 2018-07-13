#pragma once
#include "librairies.h"
#include "structs.h"
#include "srv_router.h"
#include "commands.h"

void handle_cmd(t_game_info *game_info, zsock_t *router, t_command *command);
