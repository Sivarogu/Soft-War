#include "commands.h"

void action_ping(t_game_info *game_info, zsock_t *router, t_command *command)
{
    (void)game_info;
    printf("Commande : %s from %s\n", command->name, zframe_strhex(command->identity));
    send_response(router, command->identity, "ok", "pong");
}

void action_identify(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *existing_player;
    char *cmd_identity;
    t_player *player;
    int nb_player;

    printf("Commande : %s from %s with params : %s\n", command->name, zframe_strhex(command->identity), command->params);
    if (command->params == NULL) {
        send_response(router, command->identity, "ko", "no identifier sent");
        return;
    } else if (identity_already_exists(game_info, command->params)) {
        send_response(router, command->identity, "ko", "identity already exists");
        return;
    }
    cmd_identity = zframe_strhex(command->identity);
    if ((existing_player = get_player_by_sock_id(game_info, cmd_identity))) {
        free(existing_player->id);
        existing_player->id = strdup(command->params);
        send_response(router, command->identity, "ok", "null");
    } else if ((nb_player = get_nb_player(game_info)) >= 4) {
        send_response(router, command->identity, "ko", "game full");
    } else if (game_info->game_status != PENDING) {
        send_response(router, command->identity, "ko", "null");
    } else {
        player = new_player(zframe_strhex(command->identity), strdup(command->params), game_info);
        init_player_location(player, game_info->map_size, nb_player + 1);
        send_response(router, command->identity, "ok", "null");
    }
    free(cmd_identity);
}

void action_leave(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;

    printf("Commande : %s from %s with params : %s\n", command->name, zframe_strhex(command->identity), command->params);
    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))) {
        destroy_player(current_player, game_info);
        send_response(router, command->identity, "ok", "null");
    } else {
        send_response(router, command->identity, "ko", "null");
    }
    free(cmd_identity);
}
