#pragma once
#include "librairies.h"
#include "structs.h"
#include "srv_router.h"

void action_ping(t_game_info *game_info, zsock_t *router, t_command *command);
void action_identify(t_game_info *game_info, zsock_t *router, t_command *command);
