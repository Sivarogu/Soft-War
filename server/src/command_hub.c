#include "command_hub.h"

static const t_action action_list[3] = {
    {"ping", &action_ping},
    {"identify", &action_identify},
    {NULL, NULL}
};

void handle_cmd(t_game_info *game_info, zsock_t *router, t_command *command)
{
    int index;
    bool cmd_found = false;

    index = 0;
    while (action_list[index].name)
    {
        if (!strcmp(action_list[index].name, command->name))
        {
            action_list[index].exec(game_info, router, command);
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
