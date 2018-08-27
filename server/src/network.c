#include "network.h"
#include "global_config.h"
#include "game.h"

static json_t *json_encode_player(t_player *player) {
	json_t *json_player;

	json_player = json_object();
	json_object_set_new(json_player, "name", json_string(player->id));
	json_object_set_new(json_player, "x", json_integer(player->x));
	json_object_set_new(json_player, "y", json_integer(player->y));
	json_object_set_new(json_player, "energy", json_integer(player->energy));
	json_object_set_new(json_player, "looking", json_integer(player->looking));
	return json_player;
}

static json_t *json_encode_energy_cell(t_energy_cell *energy) {
	json_t *json_energy;

	json_energy = json_object();
	json_object_set_new(json_energy, "x", json_integer(energy->x));
	json_object_set_new(json_energy, "y", json_integer(energy->y));
	json_object_set_new(json_energy, "value", json_integer(energy->energy));
	return json_energy;
}

static json_t *json_encode_cycle()
{
	json_t *cycle_json;
	json_t *players_json;
	json_t *energy_cells_json;

	players_json = json_array();
	for (t_player *player = game_info.players; player; player = player->next)
		json_array_append_new(players_json, json_encode_player(player));

	energy_cells_json = json_array();
	for (t_energy_cell *energy_cell = game_info.energy_cells; energy_cell; energy_cell = energy_cell->next)
		json_array_append_new(energy_cells_json, json_encode_energy_cell(energy_cell));

	cycle_json = json_object();
	json_object_set_new(cycle_json, "map_size", json_integer(global_config.map_size));
	json_object_set_new(cycle_json, "game_status", json_integer(game_info.game_status));
	json_object_set_new(cycle_json, "players", players_json);
	json_object_set_new(cycle_json, "energy_cells", energy_cells_json);

	return cycle_json;
}

int publish(zsock_t *socket, t_notification_type type, json_t *data) {
	json_t *notification_json;
	char *notification_str;

	notification_json = json_object();
	json_object_set_new(notification_json, "notification_type", json_integer(type));
	json_object_set_new(notification_json, "data", data);
	BIND_MZERO(notification_str = json_dumps(notification_json, JSON_COMPACT), "could not JSON encode cycle");
	free(notification_json);
	BIND_MTRUE(zstr_send(socket, notification_str), "could not send notification");
	free(notification_str);
	return 0;
}

int publish_cycle(zsock_t *socket) {
	json_t *cycle_json;

	pthread_mutex_lock(&game_info_mutex);
	BIND_MZERO(cycle_json = json_encode_cycle(), "could not JSON encode cycle");
	pthread_mutex_unlock(&game_info_mutex);

	BIND_NEG(publish(socket, NOTIFICATION_TYPE_CYCLE_INFO, cycle_json));
	free(cycle_json);
	return 0;
}


int response_send(zsock_t *socket, zframe_t *identity, const char *result, const char *data) {
	zframe_t *empty;
	zframe_t *zcontent;
	char *content;
	zmsg_t *response;

	empty = zframe_new_empty();
	BIND_ZERO((content = buildstr("%s|%s", result, data)));
	zcontent = zframe_from(content);
	response = zmsg_new();
	zmsg_prepend(response, &identity);
	zmsg_append(response, &empty);
	zmsg_append(response, &zcontent);
	zmsg_send(&response, socket);
	zmsg_destroy(&response);
	zframe_destroy(&identity);
	zframe_destroy(&empty);
	zframe_destroy(&zcontent);
	free(content);
	return 0;
}

t_command *request_receive(zsock_t *router) {
	char delimiter[2] = "|";
	zmsg_t *rcv_data;
	zframe_t *empty;
	zframe_t *zmessage;
	char *message;
	t_command *command;

	rcv_data = zmsg_recv(router);
	command = malloc(sizeof(t_command));
	command->identity = zmsg_pop(rcv_data);
	empty = zmsg_pop(rcv_data);
	zmessage = zmsg_pop(rcv_data);
	zmsg_destroy(&rcv_data);
	message = zframe_strdup(zmessage);
	command->name = strtok_r(message, delimiter, &message);
	command->params = strtok_r(NULL, delimiter, &message);
	zframe_destroy(&empty);
	zframe_destroy(&zmessage);
	return command;
}
