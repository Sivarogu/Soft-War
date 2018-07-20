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
        } while (!is_pos_available(game_info, x, y, true));
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
    if (player_rank == 1 || player_rank == 4)
        player->x = 0;
    else
        player->x = map_size - 1;
    if (player_rank < 3)
        player->y = 0;
    else
        player->y = map_size - 1;
}

int is_pos_available(t_game_info *game_info, int x, int y, bool include_nrg) {
    if (x < 0 || x >= game_info->map_size || y < 0 || y >= game_info->map_size)
        return 0;
    t_player *player = game_info->first_player;
    while (player != NULL) {
        if (player->x == x && player->y == y)
            return 0;
        player = player->next;
    }
    if (include_nrg) {
        t_energy *energy = game_info->first_energy;
        while (energy != NULL) {
            if (energy->x == x && energy->y == y)
                return 0;
            energy = energy->next;
        }
    }
    return 1;
}
