#pragma once
#include <czmq.h>

typedef struct {
    zframe_t *identity;
    char *name;
    char *params;
} t_command;

typedef int (*t_action)(zsock_t *router, t_command *command);

typedef struct {
    char *name;
    t_action exec;
} t_action_smap_entry;


void command_destroy(t_command *command);
int command_handle(zsock_t *router, t_command *command, const t_action_smap_entry *action_list);
int command_handle_lobby(zsock_t *router, t_command *command);
int command_handle_game(zsock_t *router, t_command *command);
