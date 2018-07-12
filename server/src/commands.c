#include "commands.h"

void action_ping(t_game_info *game_info, zsock_t *router, t_command *command)
{
    (void)game_info;
    printf("Commande : %s from %s\n", command->name, zframe_strhex(command->identity));
    send_response(router, command->identity, "ok", "pong");
}

void action_identify(t_game_info *game_info, zsock_t *router, t_command *command)
{
    char *cmd_identity;

    t_player *existing_player = NULL;
    t_player *player = game_info->first_player;
    int nb_player = 0;
    printf("Commande : %s from %s with params : %s\n", command->name, zframe_strhex(command->identity), command->params);
    if (command->params == NULL) {
        send_response(router, command->identity, "ko", "no identifier sent");
        return;
    }
    char *cmd_identity = zframe_strhex(command->identity);
    while (player != NULL && ++nb_player < 4) {
        if (existing_player == NULL
            && strcmp(player->socket_id, cmd_identity) == 0) {
            existing_player = player;
        }
        if (strcmp(player->id, command->params) == 0) {
            send_response(router, command->identity, "ko", "identity already exists");
            free(cmd_identity);
            return;
        }
        player = player->next;
    }
    if (existing_player != NULL) {
        existing_player->id = strdup(command->params);
        send_response(router, command->identity, "ok", command->params);
    } else if (nb_player == 4) {
        send_response(router, command->identity, "ko", "game full");
    } else {
        player = new_player(zframe_strhex(command->identity), strdup(command->params), game_info);
        //TODO : add player init location
        send_response(router, command->identity, "ok", command->params);
    }
    free(cmd_identity);
}
