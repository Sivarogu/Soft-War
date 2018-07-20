#pragma once
#include "librairies.h"
#include "structs.h"
#include "srv_router.h"
#include "game_info.h"

void action_ping(t_game_info *game_info, zsock_t *router, t_command *command);
void action_identify(t_game_info *game_info, zsock_t *router, t_command *command);
void action_leave(t_game_info *game_info, zsock_t *router, t_command *command);
void action_selfid(t_game_info *game_info, zsock_t *router, t_command *command);
void action_selfstats(t_game_info *game_info, zsock_t *router, t_command *command);
void action_next(t_game_info *game_info, zsock_t *router, t_command *command);
void action_looking(t_game_info *game_info, zsock_t *router, t_command *command);
void moving_actions(t_game_info *game_info, zsock_t *router, t_command *command, enum LookingDir moving_dir, int looking_dir_change);
void action_forward(t_game_info *game_info, zsock_t *router, t_command *command);
void action_backward(t_game_info *game_info, zsock_t *router, t_command *command);
void action_leftfwd(t_game_info *game_info, zsock_t *router, t_command *command);
void action_backward(t_game_info *game_info, zsock_t *router, t_command *command);
void looking_actions(t_game_info *game_info, zsock_t *router, t_command *command, int looking_dir_change);
void action_left(t_game_info *game_info, zsock_t *router, t_command *command);
void action_right(t_game_info *game_info, zsock_t *router, t_command *command);
void action_jump(t_game_info *game_info, zsock_t *router, t_command *command);
void action_gather(t_game_info *game_info, zsock_t *router, t_command *command);
void action_inspect(t_game_info *game_info, zsock_t *router, t_command *command);
void action_watch(t_game_info *game_info, zsock_t *router, t_command *command);
void action_attack(t_game_info *game_info, zsock_t *router, t_command *command);
