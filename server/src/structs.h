#pragma once
#include "librairies.h"

typedef struct s_command {
    zframe_t *identity;
    char *name;
    char *params;
} t_command;

typedef struct s_player {
    char *id;
    char *socket_id;
    int energy;
    int actions;
    int x;
    int y;
    int looking;
    int stunned;
    struct s_player *next;
    struct s_player *prev;
} t_player;

typedef struct s_energy {
    int value;
    int x;
    int y;
    struct s_energy *next;
    struct s_energy *prev;
} t_energy;

typedef struct s_game_info {
    int map_size;
    int game_status;
    pthread_mutex_t mutex_game;
    t_player *first_player;
    t_energy *first_energy;
} t_game_info;

typedef struct s_action {
    char *name;
    void (*exec)(t_game_info *game_info, zsock_t *router, t_command *command);
} t_action;

t_game_info *new_game_info();
t_player *new_player(char *socket_id, char *identity, t_game_info *game_info);
t_energy *new_energy(t_game_info *game_info);

t_player *get_player_by_sock_id(t_game_info *game_info, char *sock_id);

void destroy_game_info(t_game_info *game_info);
void destroy_player(t_player * player, t_game_info * game_info);
void destroy_energy(t_energy * energy, t_game_info * game_info);
void destroy_command(t_command * command);
