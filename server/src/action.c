#include "action.h"
#include "utils.h"
#include "game.h"
#include "network.h"

int action_ping(zsock_t *router, t_command *command) {
	response_send(router, command->identity, "ok", "pong");
	return 0;
}

int action_identify(zsock_t *router, t_command *command) {
	t_player *existing_player;
	char *cmd_identity;
	t_player *player;
	size_t nb_player;

	if (command->params == NULL) {
		response_send(router, command->identity, "ko", "no identifier sent");
		return 0;
	} else if (identity_already_exists(command->params)) {
		response_send(router, command->identity, "ko", "identity already exists");
		return 0;
	}
	cmd_identity = zframe_strhex(command->identity);
	if ((existing_player = get_player_by_sock_id(cmd_identity))) {
		free(existing_player->id);
		existing_player->id = strdup(command->params);
		response_send(router, command->identity, "ok", "null");
	} else if ((nb_player = get_nb_player(game_info)) >= 4) {
		response_send(router, command->identity, "ko", "game full");
	} else if (game_info.game_status != GAME_STATUS_PENDING) {
		response_send(router, command->identity, "ko", "null");
	} else {
		player = player_new(zframe_strhex(command->identity), strdup(command->params));
		init_player_location(player, game_info.map_size, nb_player + 1);
		response_send(router, command->identity, "ok", "null");
	}
	free(cmd_identity);
	return 0;
}

int action_leave(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))) {
		player_destroy(current_player);
		response_send(router, command->identity, "ok", "null");
	} else {
		response_send(router, command->identity, "ko", "null");
	}
	free(cmd_identity);
	return 0;
}

int action_selfid(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity)))
		response_send(router, command->identity, "ok", current_player->id);
	else
		response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

int action_selfstats(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;
	char energy_str[12];

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))) {
		sprintf(energy_str, "%zu", current_player->energy);
		response_send(router, command->identity, "ok", energy_str);
	} else
		response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

int action_next(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned) {
		current_player->stunned = true;
		current_player->action_points = 0;
		response_send(router, command->identity, "ok", "null");
	} else {
		response_send(router, command->identity, "ko", "null");
	}
	free(cmd_identity);
	return 0;
}

int action_looking(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;
	char looking_str[2];

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned) {
		snprintf(looking_str, 2, "%d", current_player->looking);
		response_send(router, command->identity, "ok", looking_str);
	} else
		response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

static int moving_actions(zsock_t *router, t_command *command, t_direction moving_dir, int looking_dir_change) {
	t_player *current_player;
	char *cmd_identity;
	size_t new_x, new_y, action_points;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned) {
		action_points = (moving_dir == DIRECTION_UP && !looking_dir_change) ? 1 : 2;
		new_x = comp_new_pos(current_player, moving_dir, 1, true);
		new_y = comp_new_pos(current_player, moving_dir, 1, false);
		if (current_player->action_points >= action_points && !is_pos_occupied(new_x, new_y, false)) {
			current_player->looking = (current_player->looking + looking_dir_change + 4) % 4;
			current_player->action_points -= action_points;
			current_player->x = new_x;
			current_player->y = new_y;
			response_send(router, command->identity, "ok", "null");
			free(cmd_identity);
			return 0;
		}
	}
	response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

int action_forward(zsock_t *router, t_command *command) {
	return moving_actions(router, command, DIRECTION_UP, 0);
}

int action_backward(zsock_t *router, t_command *command) {
	return moving_actions(router, command, DIRECTION_DOWN, 0);
}

int action_leftfwd(zsock_t *router, t_command *command) {
	return moving_actions(router, command, DIRECTION_LEFT, -1);
}

int action_rightfwd(zsock_t *router, t_command *command) {
	return moving_actions(router, command, DIRECTION_RIGHT, 1);
}

static int looking_actions(zsock_t *router, t_command *command, t_direction looking_dir_change) {
	t_player *current_player;
	char *cmd_identity;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned
		&& current_player->action_points >= 1) {
		current_player->looking = (current_player->looking + looking_dir_change + 4) % 4;
		current_player->action_points--;
		response_send(router, command->identity, "ok", "null");
	} else
		response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

int action_left(zsock_t *router, t_command *command) {
	return looking_actions(router, command, -1);
}

int action_right(zsock_t *router, t_command *command) {
	return looking_actions(router, command, 1);
}

int action_jump(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;
	int new_x, new_y;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned
		&& current_player->energy > 0) {
		new_x = comp_new_pos(current_player, DIRECTION_UP, 2, true);
		new_y = comp_new_pos(current_player, DIRECTION_UP, 2, false);
		if (!is_pos_occupied(new_x, new_y, false)) {
			current_player->energy -= 2;
			current_player->x = new_x;
			current_player->y = new_y;
			response_send(router, command->identity, "ok", "null");
			free(cmd_identity);
			return 0;
		}
	}
	response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

int action_gather(zsock_t *router, t_command *command) {
	t_player *current_player;
	t_energy_cell *energy_cell;
	char *cmd_identity;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned
		&& current_player->action_points >= 2) {
		energy_cell = game_info.energy_cells;
		while (energy_cell) {
			if (energy_cell->x == current_player->x && energy_cell->y == current_player->y) {
				current_player->action_points -= 2;
				current_player->energy += energy_cell->energy;
				energy_cell_destroy(energy_cell);
				response_send(router, command->identity, "ok", "null");
				free(cmd_identity);
				return 0;
			}
			energy_cell = energy_cell->next;
		}
	}
	response_send(router, command->identity, "ko", "null");
	free(cmd_identity);
	return 0;
}

int action_inspect(zsock_t *router, t_command *command) {
	t_player *player = game_info.players;
	t_player *current_player;
	char energy_str[12];
	char *cmd_identity;

	if (command->params == NULL) {
		response_send(router, command->identity, "ko", "null");
		return 0;
	}
	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned
		&& current_player->action_points >= 1) {
		while (player != NULL)
		{
			if (strcmp(player->id, command->params) == 0)
			{
				current_player->action_points--;
				sprintf(energy_str, "%zu", player->energy);
				response_send(router, command->identity, "ok", energy_str);
				return 0;
			}
			player = player->next;
		}
	}
	response_send(router, command->identity, "ko", "null");
	return 0;
}

int action_watch(zsock_t *router, t_command *command) {
	t_player *current_player;
	char *cmd_identity;
	char *vision[5];
	char *vision_concat = NULL;

	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned) {
		get_player_vision(*current_player, vision);
		vision_concat = concat_vision_string(vision);
		sprintf(vision_concat, "[\"%s\",\"%s\",\"%s\",\"%s\"]", vision[0], vision[1], vision[2], vision[3]);
		response_send(router, command->identity, "ok", vision_concat);
		free(vision_concat);
	} else {
		response_send(router, command->identity, "ko", "null");
	}
	return 0;
}

int action_attack(zsock_t *router, t_command *command) {
	t_player *current_player;
	t_player *targeted_player;
	char *cmd_identity;
	char *vision[5];

	int index = 0;
	cmd_identity = zframe_strhex(command->identity);
	if ((current_player = get_player_by_sock_id(cmd_identity))
		&& !current_player->stunned
		&& current_player->action_points >= 1
		&& current_player->energy > 0) {
		current_player->action_points--;
		current_player->energy--;
		get_player_vision(*current_player, vision);
		while (vision[index]){
			if ((targeted_player = get_player_by_id(vision[index]))) {
				targeted_player->stunned = 3;
			}
			index++;
		}
		response_send(router, command->identity, "ok", "null");
	} else {
		response_send(router, command->identity, "ko", "null");
	}
	return 0;
}
