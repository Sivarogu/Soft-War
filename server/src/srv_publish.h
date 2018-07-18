#pragma once
#include "librairies.h"
#include "structs.h"
#include "game_info.h"
#include "json_encoder.h"

void *start_publish(void *game_info);
void handle_dead_players(t_game_info *game_info, zsock_t *publish);
