#include "game_info.h"

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
    if (player_rank == 1 || player_rank == 4)
        player->x = 0;
    else
        player->x = map_size - 1;
    if (player_rank < 3)
        player->y = 0;
    else
        player->y = map_size - 1;
}

int is_pos_available(t_game_info *game_info, int x, int y) {
    if (x < 0 || x >= game_info->map_size || y < 0 || y >= game_info->map_size)
        return 0;
    t_player *player = game_info->first_player;
    while (player != NULL) {
        if (player->x == x && player->y == y)
            return 0;
        player = player->next;
    }
    t_energy *energy = game_info->first_energy;
    while (energy != NULL) {
        if (energy->x == x && energy->y == y)
            return 0;
        energy = energy->next;
    }
    return 1;
}
