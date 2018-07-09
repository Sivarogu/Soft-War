#include "srv_router.h"

int main()
{
	t_game_info *game_info = new_game_info();
	destroy_game_info(game_info);
	return 0;
}
