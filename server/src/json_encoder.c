#include "json_encoder.h"

char *json_encode_cycle(t_game_info *game_info, char *prefix)
{
    t_player *player = game_info->first_player;
    t_energy *energy = game_info->first_energy;
    char *json_string_wo_prefix, *json_string;
    json_t *json_cycle = json_object();
    json_t *json_players = json_array();
    json_t *json_energies = json_array();

    json_object_set_new(json_cycle, "map_size", json_integer(game_info->map_size));
    json_object_set_new(json_cycle, "game_status", json_integer(game_info->game_status));
    while (player) {
        json_array_append_new(json_players, json_encode_player(player));
        player = player->next;
    }
    while (energy) {
        json_array_append_new(json_energies, json_encode_energy(energy));
        energy = energy->next;
    }
    json_object_set_new(json_cycle, "players", json_players);
    json_object_set_new(json_cycle, "energy_cells", json_energies);
    json_string_wo_prefix = json_dumps(json_cycle, JSON_INDENT(2));
    json_string = malloc((strlen(prefix) + strlen(json_string_wo_prefix)) * sizeof(char));
    strcpy(json_string, prefix);
    strcat(json_string, json_string_wo_prefix);
    free(json_string_wo_prefix);
    return json_string;
}

json_t *json_encode_player(t_player *player)
{
    json_t *json_player = json_object();

    json_object_set_new(json_player, "name", json_string(player->id));
    json_object_set_new(json_player, "x", json_integer(player->x));
    json_object_set_new(json_player, "y", json_integer(player->y));
    json_object_set_new(json_player, "energy", json_integer(player->energy));
    json_object_set_new(json_player, "looking", json_integer(player->looking));
    return json_player;
}

json_t *json_encode_energy(t_energy *energy)
{
    json_t *json_energy = json_object();

    json_object_set_new(json_energy, "x", json_integer(energy->x));
    json_object_set_new(json_energy, "y", json_integer(energy->y));
    json_object_set_new(json_energy, "value", json_integer(energy->value));
    return json_energy;
}
