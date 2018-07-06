#include "command_hub.h"

static const t_action action_list[2] = {
    {"ping", &action_ping},
    {NULL, NULL}
};

void handle_cmd(zsock_t *router, t_command *command)
{
    int index;
    bool cmd_found = false;

    index = 0;
    while (action_list[index].name)
    {
        if (!strcmp(action_list[index].name, command->name))
        {
            action_list[index].exec(router, command);
            cmd_found = true;
            break;
        }
        index++;
    }
    if (!cmd_found)
    {
        send_response(router, command->identity, "ko", "null");
    }
    destroy_command(command);
}

void destroy_command(t_command *command)
{
    free(command->name);
    free(command->params);
    free(command);
}
