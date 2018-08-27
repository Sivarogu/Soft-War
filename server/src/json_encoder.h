#pragma once
#include "librairies.h"
#include "structs.h"

char *json_encode_cycle(t_game_info *game_info, char *prefix);
json_t *json_encode_player(t_player *player);
json_t *json_encode_energy(t_energy *energy);
