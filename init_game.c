#include <stdlib.h>
#include "types.h"
#include "time.h"

void	init_game(t_game *game, t_shape* current)
{
	*game = (t_game){
		.board = {0},
		.game_on = true,
		.score = 0,
		.turn_duration = 400000,
		.duration_decreasement = 1000,
	};

	*current = (t_shape){0};

	srand(time(0));
	game->turn_started_at = get_current_time();
}
