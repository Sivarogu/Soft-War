#pragma once
#include <czmq.h>
#include <jansson.h>
#include "command.h"

typedef enum {
	NOTIFICATION_TYPE_CYCLE_INFO,
	NOTIFICATION_TYPE_GAME_STARTED,
	NOTIFICATION_TYPE_GAME_FINISHED,
	NOTIFICATION_TYPE_CLIENT_DEAD,
	NOTIFICATION_TYPE_CLIENT_WIN
} t_notification_type;

int publish(zsock_t *socket, t_notification_type type, json_t *data, char *channel);
int publish_cycle(zsock_t *socket);
int response_send(zsock_t *socket, zframe_t *identity, const char *result, const char *data);
t_command *request_receive(zsock_t *router);
