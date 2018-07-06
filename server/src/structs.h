#pragma once
#include "librairies.h"

typedef struct s_command {
    zframe_t *identity;
    char *name;
    char *params;
} t_command;

typedef struct s_action {
    char *name;
    void (*exec)(zsock_t *router, t_command *command);
} t_action;
