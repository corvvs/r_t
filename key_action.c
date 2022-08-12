#include "types.h"
#include "shape.h"

void	place_shape_to_board(t_board board, const t_shape* shape)
{
	for (int i = 0; i < shape->width; i++) {
		for (int j = 0; j < shape->width; j++) {
			board[shape->row + i][shape->col + j] |= shape->array[i][j];
		}
	}
}

static int		remove_filled_lines(t_game *game) {
	int count = 0;
	for (int n = 0; n < R; n++) {
		int blocks = 0;
		for (int m = 0; m < C; m++) {
			blocks += game->board[n][m];
		}
		if (blocks == C) {
			count++;
			int k;
			for (k = n; k >= 1; k--)
				memcpy(game->board[k], game->board[k - 1], sizeof(char) * C);
			memset(game->board[k], 0, sizeof(char) * C);
			game->turn_duration -= game->duration_decreasement;
			game->duration_decreasement -= 1;
		}
	}
	return count;
}

static bool	check_collision(t_board board, const t_shape *shape)
{
	char **array = shape->array;
	for (int i = 0; i < shape->width; i++) {
		for (int j = 0; j < shape->width; j++) {
			if ((shape->col + j < 0 || shape->col + j >= C || shape->row + i >= R)) {
				if (array[i][j])
					return true;
				
			}
			else if (board[shape->row + i][shape->col + j] && array[i][j])
				return true;
		}
	}
	return false;
}

void	drop_new_shape(t_game *game, t_shape *current)
{
	*current = create_shape();
	if (check_collision(game->board, current)) {
		game->game_on = false;
	}
}

static int	move_down_shape(t_game *game, t_shape* temp, t_shape* current)
{
	temp->row++;
	if (!check_collision(game->board, temp)) {
		current->row++;
		return 0;
	} else {
		place_shape_to_board(game->board, current);
		int removed_lines = remove_filled_lines(game);
		drop_new_shape(game, current);
		return removed_lines;
	}
}

void	perform_quicken(t_game *game, t_shape *current)
{
	t_shape temp = duplicate_shape(current);
	int removed_lines = move_down_shape(game, &temp, current);
	game->score += 100 * removed_lines;
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
