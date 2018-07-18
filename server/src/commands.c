#include "commands.h"

void action_ping(t_game_info *game_info, zsock_t *router, t_command *command)
{
    (void)game_info;
    send_response(router, command->identity, "ok", "pong");
}

void action_identify(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *existing_player;
    char *cmd_identity;
    t_player *player;
    int nb_player;

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

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))) {
        destroy_player(current_player, game_info);
        send_response(router, command->identity, "ok", "null");
    } else {
        send_response(router, command->identity, "ko", "null");
    }
    free(cmd_identity);
}

void action_selfid(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity)))
        send_response(router, command->identity, "ok", current_player->id);
    else
        send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void action_selfstats(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;
    char energy_str[12];

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))) {
        sprintf(energy_str, "%d", current_player->energy);
        send_response(router, command->identity, "ok", energy_str);
    } else
        send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void action_next(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned) {
        current_player->stunned = 1;
        current_player->actions = 0;
        send_response(router, command->identity, "ok", "null");
    } else {
        send_response(router, command->identity, "ko", "null");
    }
    free(cmd_identity);
}

void action_looking(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;
    char looking_str[2];

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned) {
        snprintf(looking_str, 2, "%d", current_player->looking);
        send_response(router, command->identity, "ok", looking_str);
    } else
        send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void moving_actions(t_game_info *game_info, zsock_t *router, t_command *command, enum LookingDir moving_dir, int looking_dir_change)
{
    t_player *current_player;
    char *cmd_identity;
    int new_x, new_y, action_points;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned) {
        action_points = (moving_dir == UP && !looking_dir_change) ? 1 : 2;
        new_x = comp_new_pos(current_player, moving_dir, 1, true);
        new_y = comp_new_pos(current_player, moving_dir, 1, false);
        if (current_player->actions >= action_points
            && !is_pos_occupied(game_info, new_x, new_y, false)) {
            current_player->looking = (current_player->looking + looking_dir_change + 4) % 4;
            current_player->actions -= action_points;
            current_player->x = new_x;
            current_player->y = new_y;
            send_response(router, command->identity, "ok", "null");
            free(cmd_identity);
            return;
        }
    }
    send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void action_forward(t_game_info *game_info, zsock_t *router, t_command *command)
{
    moving_actions(game_info, router, command, UP, 0);
}

void action_backward(t_game_info *game_info, zsock_t *router, t_command *command)
{
    moving_actions(game_info, router, command, DOWN, 0);
}

void action_leftfwd(t_game_info *game_info, zsock_t *router, t_command *command)
{
    moving_actions(game_info, router, command, UP, -1);
}

void action_rightfwd(t_game_info *game_info, zsock_t *router, t_command *command)
{
    moving_actions(game_info, router, command, UP, 1);
}

void looking_actions(t_game_info *game_info, zsock_t *router, t_command *command, int looking_dir_change)
{
    t_player *current_player;
    char *cmd_identity;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned
        && current_player->actions >= 1) {
        current_player->looking = (current_player->looking + looking_dir_change + 4) % 4;
        current_player->actions--;
        send_response(router, command->identity, "ok", "null");
    } else
        send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void action_left(t_game_info *game_info, zsock_t *router, t_command *command)
{
    looking_actions(game_info, router, command, -1);
}

void action_right(t_game_info *game_info, zsock_t *router, t_command *command)
{
    looking_actions(game_info, router, command, 1);
}

void action_jump(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;
    int new_x, new_y;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned
        && current_player->energy > 0) {
        new_x = comp_new_pos(current_player, UP, 2, true);
        new_y = comp_new_pos(current_player, UP, 2, false);
        if (!is_pos_occupied(game_info, new_x, new_y, false)) {
            current_player->energy -= 2;
            current_player->x = new_x;
            current_player->y = new_y;
            send_response(router, command->identity, "ok", "null");
            free(cmd_identity);
            return;
        }
    }
    send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void action_gather(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    t_energy *energy;
    char *cmd_identity;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned
        && current_player->actions >= 2) {
        energy = game_info->first_energy;
        while (energy != NULL) {
            if (energy->x == current_player->x && energy->y == current_player->y) {
                current_player->actions -= 2;
                current_player->energy += energy->value;
                destroy_energy(energy, game_info);
                send_response(router, command->identity, "ok", "null");
                free(cmd_identity);
                return;
            }
            energy = energy->next;
        }
    }
    send_response(router, command->identity, "ko", "null");
    free(cmd_identity);
}

void action_inspect(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *player = game_info->first_player;
    t_player *current_player;
    char energy_str[12];
    char *cmd_identity;

    if (command->params == NULL) {
        send_response(router, command->identity, "ko", "null");
        return;
    }
    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned
        && current_player->actions >= 1) {
        while (player != NULL)
        {
            if (strcmp(player->id, command->params) == 0)
            {
                current_player->actions--;
                sprintf(energy_str, "%d", player->energy);
                send_response(router, command->identity, "ok", energy_str);
                return;
            }
            player = player->next;
        }
    }
    send_response(router, command->identity, "ko", "null");
}

void action_watch(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    char *cmd_identity;
    char *vision[5];
    char *vision_concat = NULL;

    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned) {
        get_player_vision(*current_player, game_info, vision);
        vision_concat = concat_vision_string(vision);
        sprintf(vision_concat, "[\"%s\",\"%s\",\"%s\",\"%s\"]", vision[0], vision[1], vision[2], vision[3]);
        send_response(router, command->identity, "ok", vision_concat);
    } else {
        send_response(router, command->identity, "ko", "null");
    }
}

void action_attack(t_game_info *game_info, zsock_t *router, t_command *command)
{
    t_player *current_player;
    t_player *targeted_player;
    char *cmd_identity;
    char *vision[5];

    int index = 0;
    cmd_identity = zframe_strhex(command->identity);
    if ((current_player = get_player_by_sock_id(game_info, cmd_identity))
        && !current_player->stunned
        && current_player->actions >= 1
        && current_player->energy > 0) {
        current_player->actions--;
        current_player->energy--;
        get_player_vision(*current_player, game_info, vision);
        while (vision[index]){
            if ((targeted_player = get_player_by_id(game_info, vision[index]))) {
                targeted_player->stunned = 3;
            }
            index++;
        }
        send_response(router, command->identity, "ok", "null");
    } else {
        send_response(router, command->identity, "ko", "null");
    }
}
