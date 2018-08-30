#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "log.h"
#include "config_loader.h"
#include "global_config.h"
#include "thread_publish.h"
#include "thread_router.h"
#include "game.h"

int main(int argc, const char **argv) {
	pthread_t thread_publish;
	pthread_t thread_router;

	srand(time(NULL));
	BIND_MTRUE(pthread_mutex_init(&game_info_mutex, NULL) != 0, "mutex init failed\n");
	BIND_MTRUE(pthread_cond_init(&game_info_mutex_start_cond, NULL) != 0, "mutex condition init failed\n");
	BIND_NEG(global_config_of_args(argc, argv));
	BIND_NEG(global_config_of_config_file());
	BIND_NEG(log_init());
	BIND_NEG(log_info("config loaded; rep_port: %d, pub_port: %d", global_config.rep_port, global_config.pub_port));
	BIND_MTRUE(pthread_create(&thread_publish, NULL, thread_publish_run, NULL) != 0, "could not instantiate the publish thread");
	BIND_MTRUE(pthread_create(&thread_router, NULL, thread_router_run, NULL) != 0, "could not instantiate the router thread");
	BIND_TRUE(pthread_join(thread_publish, NULL) != 0);
	BIND_TRUE(pthread_join(thread_router, NULL) != 0);
	pthread_mutex_destroy(&game_info_mutex);
	return 0;
}
