#include "thread_publish.h"
#include <czmq.h>
#include "global_config.h"
#include "utils.h"
#include "network.h"

static int thread_publish_start() {
	zsock_t *socket;

	BIND_ZERO((socket = zsock_new(ZMQ_PUB)));
	BIND_NEG(zsock_bind(socket, "tcp://*:%zu", global_config.pub_port));
	while (!zsys_interrupted) {
		BIND_NEG(publish_cycle(socket));
		log_debug("frame sent");
		zclock_sleep(global_config.cycle_interval / 1000);
	}
	zsock_destroy(&socket);
	return 0;
}

void *thread_publish_run() {
	if (thread_publish_start() < 0) {
		log_error("the publish thread failed. aborting...");
	}
	pthread_exit(NULL);
}
