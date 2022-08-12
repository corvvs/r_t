#include <string.h>
#include "types.h"
#include "shape.h"
#include "key_action.h"

// ブロックで埋まった行を削除し, 削除した行の数を返す
static int	remove_filled_lines(t_game *game) {
	int n_removed_lines = 0;
	for (int n = 0; n < R; n++) {
		int blocks = 0;
		for (int m = 0; m < C; m++) {
			blocks += game->board[n][m];
		}
		if (blocks != C) { continue; }

		n_removed_lines += 1;
		int k;
		for (k = n; k >= 1; k--)
			memcpy(game->board[k], game->board[k - 1], sizeof(char) * C);
		memset(game->board[k], 0, sizeof(char) * C);
		game->turn_duration -= game->duration_decreasement;
		game->duration_decreasement -= 1;
	}
	return n_removed_lines;
}

// shape が board の端や既存のブロックにぶつかっているかどうか判定
static bool	check_collision(t_board board, const t_shape *shape)
{
	char **array = shape->array;
	for (int i_shape = 0; i_shape < shape->width; i_shape += 1) {
		for (int j_shape = 0; j_shape < shape->width; j_shape += 1) {
			const bool	is_shape_filled = !!array[i_shape][j_shape];
			if (!is_shape_filled) { continue; }
			const int	i_board = i_shape + shape->row;
			const int	j_board = j_shape + shape->col;
			const bool	is_out_of_bounds = (j_board < 0 || C <= j_board || R <= i_board);
			if (is_out_of_bounds) {
				// アウト: shape の埋まっている位置が board からはみ出している(ただし上へのはみ出しは不問)
				return true;
			}
			const bool	is_board_filled = !!board[i_board][j_board];
			if (is_board_filled) {
				// アウト: shape と board で同じ位置が埋まっている
				return true;
			}
		}
	}
	return false;
}

// ブロックを落下させ, それにより消去された行の総数を返す
static int	move_down_shape(t_game *game, t_shape* temp, t_shape* current)
{
	temp->row++;
	if (!check_collision(game->board, temp)) {
		current->row++;
		return 0;
	}
	place_shape_to_board(game->board, current);
	const int removed_lines = remove_filled_lines(game);
	drop_new_shape(game, current);
	return removed_lines;
}

void	place_shape_to_board(t_board board, const t_shape* shape)
{
	for (int i = 0; i < shape->width; i++) {
		for (int j = 0; j < shape->width; j++) {
			board[shape->row + i][shape->col + j] |= shape->array[i][j];
		}
	}
}

void	drop_new_shape(t_game *game, t_shape *current)
{
	*current = create_shape();
	if (check_collision(game->board, current)) {
		game->game_on = false;
	}
}

void	perform_quicken(t_game *game, t_shape *current)
{
	t_shape temp = duplicate_shape(current);
	const int removed_lines = move_down_shape(game, &temp, current);
	game->score += removed_lines * SCORE_PER_LINE;
	destroy_shape(&temp);
}

void	perform_move_right(t_game *game, t_shape *current)
{
	t_shape temp = duplicate_shape(current);
	temp.col++;
	if (!check_collision(game->board, &temp))
		current->col++;
	destroy_shape(&temp);
}

void	perform_move_left(t_game *game, t_shape *current)
{
	t_shape temp = duplicate_shape(current);
	temp.col--;
	if (!check_collision(game->board, &temp))
		current->col--;
	destroy_shape(&temp);
}

void	perform_rotate_clockwise(t_game *game, t_shape *current)
{
	t_shape temp = duplicate_shape(current);
	rotate_shape_clockwise(&temp);
	if (!check_collision(game->board, &temp))
		rotate_shape_clockwise(current);
	destroy_shape(&temp);
}
