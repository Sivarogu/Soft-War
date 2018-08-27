#pragma once
#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>
#define GAME_PLAYER_COUNT 4

typedef enum {
	GAME_STATUS_PENDING,
	GAME_STATUS_STARTED,
	GATE_STATUS_FINISHED
} t_game_status;

typedef enum {
	DIRECTION_LEFT,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN
} t_direction;

typedef struct s_player {
	char *id;
	char *socket_id;
	size_t x;
	size_t y;
	size_t energy;
	size_t action_points;
	t_direction looking;
	size_t stunned;
	struct s_player *prev;
	struct s_player *next;
} t_player;

typedef struct s_energy_cell {
	size_t x;
	size_t y;
	size_t energy;
	struct s_energy_cell *prev;
	struct s_energy_cell *next;
} t_energy_cell;

typedef struct {
	size_t map_size;
	t_game_status game_status;
	t_player *players;
	t_energy_cell *energy_cells;
} t_game;

t_game game_info;
pthread_mutex_t game_info_mutex;

int next_game_cycle();
size_t get_nb_player();
size_t get_nb_energy();
int identity_already_exists(char *new_id);
void init_player_location(t_player *player, size_t map_size, int player_rank);
int is_pos_occupied(size_t x, size_t y, bool include_nrg);
int comp_new_pos(t_player *player, t_direction moving_dir, int distance, bool is_X);
void get_player_vision(t_player player, char *vision[]);
char *concat_vision_string(char *vision[]);

char *get_pos_content(size_t x, size_t y);

t_player *get_player_by_sock_id(char *sock_id);
t_player *get_player_by_id(char *id);
char *get_player_id_by_pos(size_t x, size_t y);

t_player *player_new(char *socket_id, char *identity);
void player_destroy(t_player * player);
t_energy_cell *energy_cell_new();
void energy_cell_destroy(t_energy_cell * energy_cell);
