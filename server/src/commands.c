#include "commands.h"

void action_ping(zsock_t *router, t_command *command)
{
    send_response(router, command->identity, "ok", "pong");
}
