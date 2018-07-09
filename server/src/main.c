#include "srv_router.h"

int main()
{
	pthread_t thread_router;

	t_game_info *game_info = new_game_info();
	if (pthread_create(&thread_router, NULL, start_router, game_info)) {
		printf("pthread_create error");
		return EXIT_FAILURE;
	}
	// sleep(1);
	pthread_join(thread_router, NULL);
	destroy_game_info(game_info);
	return 0;
}
