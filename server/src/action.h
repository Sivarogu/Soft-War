#pragma once
#include "command.h"

int action_ping(zsock_t *router, t_command *command);
int action_identify(zsock_t *router, t_command *command);
int action_leave(zsock_t *router, t_command *command);
int action_selfid(zsock_t *router, t_command *command);
int action_selfstats(zsock_t *router, t_command *command);
int action_next(zsock_t *router, t_command *command);
int action_looking(zsock_t *router, t_command *command);
int action_forward(zsock_t *router, t_command *command);
int action_backward(zsock_t *router, t_command *command);
int action_leftfwd(zsock_t *router, t_command *command);
int action_backward(zsock_t *router, t_command *command);
int action_left(zsock_t *router, t_command *command);
int action_right(zsock_t *router, t_command *command);
int action_jump(zsock_t *router, t_command *command);
int action_gather(zsock_t *router, t_command *command);
int action_inspect(zsock_t *router, t_command *command);
int action_watch(zsock_t *router, t_command *command);
int action_attack(zsock_t *router, t_command *command);
