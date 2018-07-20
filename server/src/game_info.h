#pragma once
#include "librairies.h"
#include "structs.h"

void new_game_cycle(t_game_info *game_info);
int get_nb_player(t_game_info *game_info);
int get_nb_energy(t_game_info *game_info);
int identity_already_exists(t_game_info *game_info, char *new_id);
void init_player_location(t_player *player, int map_size, int player_rank);
int is_pos_available(t_game_info *game_info, int x, int y, bool include_nrg);
