#include "command_hub.h"

static const t_action lobby_action_list[] = {
    {"ping", &action_ping},
    {"identify", &action_identify},
    {"leave", &action_leave},
    {"selfid", &action_selfid},
    {NULL, NULL}};

static const t_action game_action_list[] = {
    {"identify", &action_identify},
    {"leave", &action_leave},
    {"selfid", &action_selfid},
    {"selfstats", &action_selfstats},
    {"next", &action_next},
    {"looking", &action_looking},
    {"forward", &action_forward},
    {"backward", &action_backward},
    {"leftfwd", &action_leftfwd},
    {"backward", &action_backward},
    {"left", &action_left},
    {"right", &action_right},
    {"jump", &action_jump},
    {"gather", &action_gather},
    {"inspect", &action_inspect},
    {"watch", &action_watch},
    {"attack", &action_attack},
    {NULL, NULL}};

void handle_lobby_cmd(t_game_info *game_info, zsock_t *router, t_command *command) {
    handle_cmd(game_info, router, command, lobby_action_list);
}

void handle_game_cmd(t_game_info *game_info, zsock_t *router, t_command *command) {
    handle_cmd(game_info, router, command, game_action_list);
}

void handle_cmd(t_game_info *game_info, zsock_t *router, t_command *command, const t_action *action_list)
{
    int index;
    bool cmd_found = false;

    index = 0;
    while (action_list[index].name)
    {
        if (!strcmp(action_list[index].name, command->name))
        {
            //TODO tester le lock et unlock
            pthread_mutex_lock(&game_info->mutex_game);
            action_list[index].exec(game_info, router, command);
            pthread_mutex_unlock(&game_info->mutex_game);
            cmd_found = true;
            break;
        }
        index++;
    }
    printf("Commande : %s from %s with params : %s\n", command->name, zframe_strhex(command->identity), command->params);
    if (!cmd_found)
        send_response(router, command->identity, "ko", "null");
    destroy_command(command);
}
