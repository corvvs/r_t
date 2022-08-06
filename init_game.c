#include "tetris.h"

void	init_game(t_game *game, t_shape* current)
{
	*game = (t_game){
		.board = {0},
		.timer = 400000,
		.on = true,
		.final = 0,
		.decrease = 1000,
	};
	*current = (t_shape){0};

	srand(time(0));
	game->updated_at = get_current_time();
	create_window();
}
