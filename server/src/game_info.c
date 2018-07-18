#include "game_info.h"

void new_game_cycle(t_game_info *game_info) {
    int x;
    int y;

    t_player *player = game_info->first_player;
    while (player != NULL)
    {
        if (player->stunned > 0)
            player->stunned--;
        player->actions = 2;
        player->energy -= 2;
        player = player->next;
    }
    if (get_nb_energy(game_info) < (game_info->map_size * game_info->map_size - get_nb_player(game_info))) {
        do
        {
            x = rand() % game_info->map_size;
            y = rand() % game_info->map_size;
        } while (is_pos_occupied(game_info, x, y, true));
        t_energy *energy = new_energy(game_info);
        energy->x = x;
        energy->y = y;
    }
}

int get_nb_player(t_game_info *game_info) {
    t_player *player = game_info->first_player;
    int nb = 0;
    while (player != NULL)
    {
        nb++;
        player = player->next;
    }
    return nb;
}

int get_nb_energy(t_game_info *game_info) {
    t_energy *energy = game_info->first_energy;
    int nb = 0;
    while (energy != NULL)
    {
        nb++;
        energy = energy->next;
    }
    return nb;
}

int identity_already_exists(t_game_info *game_info, char *new_id) {
    t_player *player = game_info->first_player;
    while (player != NULL)
    {
        if (strcmp(player->id, new_id) == 0)
            return 1;
        player = player->next;
    }
    return 0;
}

void init_player_location(t_player *player, int map_size, int player_rank) {
    player->looking = (player_rank + 1) % 4;
    if (player_rank == 1 || player_rank == 4)
        player->x = 0;
    else
        player->x = map_size - 1;
    if (player_rank < 3)
        player->y = 0;
    else
        player->y = map_size - 1;
}

int is_pos_occupied(t_game_info *game_info, int x, int y, bool include_nrg) {
    if (x < 0 || x >= game_info->map_size || y < 0 || y >= game_info->map_size)
        return 1;
    t_player *player = game_info->first_player;
    while (player != NULL) {
        if (player->x == x && player->y == y)
            return 2;
        player = player->next;
    }
    if (include_nrg) {
        t_energy *energy = game_info->first_energy;
        while (energy != NULL) {
            if (energy->x == x && energy->y == y)
                return 3;
            energy = energy->next;
        }
    }
    return 0;
}

int comp_new_pos(t_player *player, enum LookingDir moving_dir, int distance, bool is_X) {
    enum LookingDir final_dir = (player->looking + moving_dir - 1) % 4;
    if (is_X && final_dir % 2 == 0)
        return player->x + (final_dir - 1) * distance;
    else if (is_X && final_dir % 2 != 0)
        return player->x;
    else if (!is_X && final_dir % 2 == 1)
        return player->y + (final_dir - 2) * distance;
    else
        return player->y;
}

void get_player_vision(t_player player, t_game_info *game_info, char *vision[])
{
    int x1, y1, x2, y2, x3, y3, x4, y4;
    x1 = comp_new_pos(&player, UP, 1, true);
    y1 = comp_new_pos(&player, UP, 1, false);
    vision[0] = get_pos_content(game_info, x1, y1);
    x3 = comp_new_pos(&player, UP, 2, true);
    y3 = comp_new_pos(&player, UP, 2, false);
    vision[2] = get_pos_content(game_info, x3, y3);
    player.x = x3;
    player.y = y3;
    x2 = comp_new_pos(&player, LEFT, 1, true);
    y2 = comp_new_pos(&player, LEFT, 1, false);
    vision[1] = get_pos_content(game_info, x2, y2);
    x4 = comp_new_pos(&player, RIGHT, 1, true);
    y4 = comp_new_pos(&player, RIGHT, 1, false);
    vision[3] = get_pos_content(game_info, x4, y4);
    vision[4] = NULL;
}

char *concat_vision_string(char *vision[])
{
    int total_len = 14;
    int index = 0;
    char *vision_concat;

    while (vision[index])
        total_len += strlen(vision[index++]);
    vision_concat = malloc(total_len * sizeof(char));
    strcpy(vision_concat, "[\"");
    index = 0;
    while (vision[index]) {
        strcat(vision_concat, vision[index]);
        if (vision[++index])
            strcat(vision_concat, "\",\"");
    }
    strcat(vision_concat, "\"]");
    return vision_concat;
}

char *get_pos_content(t_game_info *game_info, int x, int y)
{
    int content;

    content = is_pos_occupied(game_info, x, y, true);
    if (content == 0 || content == 1)
        return "empty";
    else if (content == 3)
        return "energy";
    else
        return get_player_id_by_pos(game_info, x, y);
}
