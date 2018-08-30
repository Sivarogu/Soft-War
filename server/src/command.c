#include "command.h"
#include "action.h"
#include "log.h"
#include "game.h"
#include "network.h"
#include "utils.h"

static const t_action_smap_entry lobby_action_map[] = {
	{"ping", action_ping},
	{"identify", action_identify},
	{"leave", action_leave},
	{"selfid", action_selfid},
{NULL, NULL}};

static const t_action_smap_entry game_action_map[] = {
	{"identify", action_identify},
	{"leave", action_leave},
	{"selfid", action_selfid},
	{"selfstats", action_selfstats},
	{"next", action_next},
	{"looking", action_looking},
	{"forward", action_forward},
	{"backward", action_backward},
	{"leftfwd", action_leftfwd},
	{"backward", action_backward},
	{"left", action_left},
	{"right", action_right},
	{"jump", action_jump},
	{"gather", action_gather},
	{"inspect", action_inspect},
	{"watch", action_watch},
	{"attack", action_attack},
{NULL, NULL}};

int command_handle_lobby(zsock_t *router, t_command *command) {
	return command_handle(router, command, lobby_action_map);
}

int command_handle_game(zsock_t *router, t_command *command) {
	return command_handle(router, command, game_action_map);
}

int command_handle(zsock_t *router, t_command *command, const t_action_smap_entry *action_map) {
	bool cmd_found = false;

	for (const t_action_smap_entry *action = action_map; action->name; action++)
		if (!strcmp(action->name, command->name)) {
			BIND_NEG(log_debug("command: %s from %s with params: %s", command->name, zframe_strhex(command->identity), command->params));
			pthread_mutex_lock(&game_info_mutex);
			BIND_NEG(action->exec(router, command));
			pthread_mutex_unlock(&game_info_mutex);
			cmd_found = true;
			break;
		}
	if (!cmd_found)
		response_send(router, command->identity, "ko", "null");
	command_destroy(command);
	return 0;
}

void command_destroy(t_command *command) {
	free(command->name);
	free(command);
}
