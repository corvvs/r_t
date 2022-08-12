#include "types.h"
#include "time.h"
#include "window.h"
#include "key_action.h"
#include <ncurses.h>

// ゲームターンが終わったどうかの判定
static int	has_over_turn(t_game *game)
{
	suseconds_t elapsed_time = get_current_time() - game->turn_started_at;
	return elapsed_time > game->turn_duration;
}

static void	print_game(t_game *game, t_shape *current)
{
	t_board print_buffer = {0};
	place_shape_to_board(print_buffer, current);
	display_to_window(print_buffer, game);
}

// キー入力に対する処理
static void	perform_key_action(t_game *game, t_shape* current)
{
	const int c = get_char();
	switch (c) {
		case KEY_QUICKEN:
			perform_quicken(game, current);
			break;
		case KEY_MOVE_RIGHT:
			perform_move_right(game, current);
			break;
		case KEY_MOVE_LEFT:
			perform_move_left(game, current);
			break;
		case KEY_ROTATE:
			perform_rotate_clockwise(game, current);
			break;
	}
	print_game(game, current);
}

// ターン経過時の処理
static void	perform_turn_end(t_game *game, t_shape* current)
{
	if (!has_over_turn(game)) { return; }
	perform_quicken(game, current);
	print_game(game, current);
	game->turn_started_at = get_current_time();
}

void		game_loop(t_game *game, t_shape* current)
{
	create_window();
	drop_new_shape(game, current);
	print_game(game, current);
	while (game->game_on) {
		perform_key_action(game, current);
		perform_turn_end(game, current);
	}
	destroy_window();
}
