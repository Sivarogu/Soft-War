#include "srv_router.h"
#include "srv_publish.h"

int main()
{
	pthread_t thread_router;
	pthread_t thread_publish;

	t_game_info *game_info = new_game_info();
	if (game_info == NULL)
		return EXIT_FAILURE;
	if (pthread_create(&thread_publish, NULL, start_publish, game_info)) {
			printf("pthread_create error");
			return EXIT_FAILURE;
	}
	if (pthread_create(&thread_router, NULL, start_router, game_info)) {
		printf("pthread_create error");
		return EXIT_FAILURE;
	}
	//TODO : check every strdup, zframe_strdup and zframe_strhex
	//TODO : return at least int for every function to handle errors
	if (pthread_join(thread_router, NULL))
		return EXIT_FAILURE;
	if (pthread_join(thread_publish, NULL))
		return EXIT_FAILURE;
	destroy_game_info(game_info);
	return 0;
}
