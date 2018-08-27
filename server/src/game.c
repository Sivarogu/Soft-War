#include "game.h"
#include <czmq.h>
#include "utils.h"
#include "global_config.h"
#include "network.h"
#include "command.h"

int game_lobby() {
	zsock_t *router;
	t_command *message;

	BIND_ZERO(router = zsock_new(ZMQ_ROUTER));
	BIND_NEG(zsock_bind(router, "tcp://*:%zu", global_config.rep_port));
	while (!zsys_interrupted && get_nb_player() < 1) {
		message = request_receive(router);
		command_handle_lobby(router, message);
	}
	return 0;
}

int next_game_cycle() {
	size_t x;
	size_t y;

	for (t_player *player = game_info.players; player; player = player->next) {
		if (player->stunned > 0)
			player->stunned--;
		player->action_points = 2;
		player->energy -= 2;
	}
	if (get_nb_energy() < (game_info.map_size * game_info.map_size - get_nb_player())) {
		do {
			x = rand() % game_info.map_size;
			y = rand() % game_info.map_size;
		} while (is_pos_occupied(x, y, true));
		t_energy_cell *energy = energy_cell_new();
		energy->x = x;
		energy->y = y;
	}
	return 0;
}

size_t get_nb_player() {
	t_player *player = game_info.players;
	size_t nb = 0;
	while (player) {
		nb++;
		player = player->next;
	}
	return nb;
}

size_t get_nb_energy() {
	t_energy_cell *energy_cell = game_info.energy_cells;
	size_t nb = 0;
	while (energy_cell) {
		nb++;
		energy_cell = energy_cell->next;
	}
	return nb;
}

int identity_already_exists(char *new_id) {
	t_player *player = game_info.players;
	while (player != NULL) {
		if (strcmp(player->id, new_id) == 0)
			return 1;
		player = player->next;
	}
	return 0;
}

void init_player_location(t_player *player, size_t map_size, int player_rank) {
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

int is_pos_occupied(size_t x, size_t y, bool include_nrg) {
	if (x >= game_info.map_size || y >= game_info.map_size)
		return 1;
	t_player *player = game_info.players;
	while (player != NULL) {
		if (player->x == x && player->y == y)
			return 2;
		player = player->next;
	}
	if (include_nrg) {
		t_energy_cell *energy = game_info.energy_cells;
		while (energy != NULL) {
			if (energy->x == x && energy->y == y)
				return 3;
			energy = energy->next;
		}
	}
	return 0;
}

int comp_new_pos(t_player *player, t_direction moving_dir, int distance, bool is_X) {
	t_direction final_dir = (player->looking + moving_dir - 1) % 4;
	if (is_X && final_dir % 2 == 0)
		return player->x + (final_dir - 1) * distance;
	else if (is_X && final_dir % 2 != 0)
		return player->x;
	else if (!is_X && final_dir % 2 == 1)
		return player->y + (final_dir - 2) * distance;
	else
		return player->y;
}

void get_player_vision(t_player player, char *vision[]) {
	int x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = comp_new_pos(&player, DIRECTION_UP, 1, true);
	y1 = comp_new_pos(&player, DIRECTION_UP, 1, false);
	vision[0] = get_pos_content(x1, y1);
	x3 = comp_new_pos(&player, DIRECTION_UP, 2, true);
	y3 = comp_new_pos(&player, DIRECTION_UP, 2, false);
	vision[2] = get_pos_content(x3, y3);
	player.x = x3;
	player.y = y3;
	x2 = comp_new_pos(&player, DIRECTION_LEFT, 1, true);
	y2 = comp_new_pos(&player, DIRECTION_LEFT, 1, false);
	vision[1] = get_pos_content(x2, y2);
	x4 = comp_new_pos(&player, DIRECTION_RIGHT, 1, true);
	y4 = comp_new_pos(&player, DIRECTION_RIGHT, 1, false);
	vision[3] = get_pos_content(x4, y4);
	vision[4] = NULL;
}

char *concat_vision_string(char *vision[]) {
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

char *get_pos_content(size_t x, size_t y) {
	int content;

	content = is_pos_occupied(x, y, true);
	if (content == 0 || content == 1)
		return "empty";
	else if (content == 3)
		return "energy";
	else
		return get_player_id_by_pos(x, y);
}

t_player *player_new(char *socket_id, char *identity) {
	t_player *player_new;

	player_new = malloc(sizeof(t_player));
	player_new->socket_id = socket_id;
	player_new->id = identity;
	player_new->energy = 50;
	player_new->action_points = 2;
	player_new->stunned = 0;
	player_new->next = NULL;
	if (game_info.players != NULL) {
		player_new->next = game_info.players;
		game_info.players->prev = player_new;
	}
	game_info.players = player_new;
	return player_new;
}

t_energy_cell *energy_cell_new() {
	t_energy_cell *new_energy_cell;

	new_energy_cell = malloc(sizeof(t_energy_cell));
	new_energy_cell->energy = 5 + rand() % (15 - 5 + 1);
	new_energy_cell->next = NULL;
	new_energy_cell->prev = NULL;
	if (game_info.energy_cells != NULL) {
		new_energy_cell->next = game_info.energy_cells;
		game_info.energy_cells->prev = new_energy_cell;
	}
	game_info.energy_cells = new_energy_cell;
	return new_energy_cell;
}

t_player *get_player_by_sock_id(char *sock_id) {
	t_player *player = game_info.players;
	while (player != NULL) {
		if (strcmp(player->socket_id, sock_id) == 0)
			return player;
		player = player->next;
	}
	return NULL;
}

t_player *get_player_by_id(char *id) {
	t_player *player = game_info.players;
	while (player != NULL) {
		if (strcmp(player->id, id) == 0)
			return player;
		player = player->next;
	}
	return NULL;
}

char *get_player_id_by_pos(size_t x, size_t y) {
	t_player *player = game_info.players;
	while (player != NULL) {
		if (player->x == x && player->y == y)
			return player->id;
		player = player->next;
	}
	return NULL;
}

void player_destroy(t_player *player) {
	free(player->id);
	free(player->socket_id);
	if (player->prev == NULL) {
		game_info.players = player->next;
	} else {
		player->prev->next = player->next;
	}
	if (player->next != NULL) {
		player->next->prev = player->prev;
	}
	free(player);
}

void energy_cell_destroy(t_energy_cell *energy) {
	if (energy->prev == NULL) {
		game_info.energy_cells = energy->next;
	} else {
		energy->prev->next = energy->next;
	}
	if (energy->next != NULL) {
		energy->next->prev = energy->prev;
	}
	free(energy);
}
