#include <stdlib.h>
#include "types.h"
#include "time.h"

void	init_game(t_game *game, t_shape* current)
{
	*game = (t_game){
		.board = {},
		.game_on = true,
		.score = 0,
		.turn_duration = INITIAL_TURN_DURATION_MS * 1000,
		.duration_decreasement = INITIAL_TURN_DECREASEMENT_MS * 1000,
	};

	*current = (t_shape){0};

	game->turn_started_at = get_current_time();
	srand(game->turn_started_at);
}
