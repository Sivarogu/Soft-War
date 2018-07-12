#include "structs.h"

t_game_info *new_game_info()
{
    t_game_info *new_game_info;

    new_game_info = malloc(sizeof(t_game_info));
    if (new_game_info == NULL)
        return NULL;
    if (pthread_mutex_init(&new_game_info->mutex_game, NULL) != 0) {
        free(new_game_info);
        return NULL;
    }
    new_game_info->first_player = NULL;
    new_game_info->game_status = 0;
    return new_game_info;
}

t_player *new_player(char *socket_id, char *identity, t_game_info *game_info)
{
    t_player *new_player;

    new_player = malloc(sizeof(t_player));
    new_player->socket_id = socket_id;
    new_player->id = identity;
    new_player->energy = 50;
    new_player->actions = 1;
    new_player->stunned = 0;
    new_player->next = NULL;
    new_player->prev = NULL;
    if (game_info->first_player != NULL) {
        new_player->next = game_info->first_player;
        game_info->first_player->prev = new_player;
    }
    game_info->first_player = new_player;
    return new_player;
}

t_energy *new_energy(t_game_info *game_info)
{
    t_energy *new_energy;

    new_energy = malloc(sizeof(t_energy));
    new_energy->next = NULL;
    new_energy->prev = NULL;
    if (game_info->first_energy != NULL)
    {
        new_energy->next = game_info->first_energy;
        game_info->first_energy->prev = new_energy;
    }
    game_info->first_energy = new_energy;
    return new_energy;
}

void destroy_game_info(t_game_info *game_info)
{
    while (game_info->first_player != NULL) {
        destroy_player(game_info->first_player, game_info);
    }
    while (game_info->first_energy != NULL) {
        destroy_energy(game_info->first_energy, game_info);
    }
    free(game_info);
}

void destroy_player(t_player *player, t_game_info *game_info)
{
    free(player->id);
    free(player->socket_id);
    if (player->prev == NULL) {
        game_info->first_player = player->next;
    } else {
        player->prev->next = player->next;
    }
    if (player->next != NULL) {
        player->next->prev = player->prev;
    }
    free(player);
}

void destroy_energy(t_energy *energy, t_game_info *game_info)
{
    if (energy->prev == NULL) {
        game_info->first_energy = energy->next;
    } else {
        energy->prev->next = energy->next;
    }
    if (energy->next != NULL) {
        energy->next->prev = energy->prev;
    }
    free(energy);
}

void destroy_command(t_command *command)
{
    free(command->name);
    free(command);
}
