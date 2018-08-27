#include <pthread.h>
#include "log.h"
#include "config_loader.h"
#include "global_config.h"
#include "thread_publish.h"
#include "game.h"

int main(int argc, const char **argv) {
	pthread_t thread_publish;

	BIND_MTRUE(pthread_mutex_init(&game_info_mutex, NULL) != 0, "mutex init failed\n");
	// srand(time(NULL));
	BIND_NEG(global_config_of_args(argc, argv, &global_config));
	log_info("config loaded; rep_port: %d, pub_port: %d", global_config.rep_port, global_config.pub_port);

	BIND_MTRUE(pthread_create(&thread_publish, NULL, thread_publish_run, NULL) != 0, "could not instantiate the publish thread");
	BIND_TRUE(pthread_join(thread_publish, NULL) != 0);
	pthread_mutex_destroy(&game_info_mutex);
	return 0;
}
