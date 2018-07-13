#pragma once
#include "librairies.h"
#include "structs.h"
#include "srv_router.h"
#include "commands.h"

void handle_lobby_cmd(t_game_info *game_info, zsock_t *router, t_command *command);
void handle_game_cmd(t_game_info *game_info, zsock_t *router, t_command *command);
void handle_cmd(t_game_info *game_info, zsock_t *router, t_command *command, const t_action *action_list);
