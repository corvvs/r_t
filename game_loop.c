#include "tetris.h"

const t_shape Tetriminoes[7] = {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};

static bool	check_placed(t_board board, const t_shape *shape)
{
	char **array = shape->array;
	for (int i = 0; i < shape->width; i++)
	{
		for (int j = 0; j < shape->width; j++)
		{
			if ((shape->col + j < 0 || shape->col + j >= C || shape->row + i >= R))
			{
				// 
				if (array[i][j])
					return false;
				
			}
			else if (board[shape->row + i][shape->col + j] && array[i][j])
				return false;
		}
	}
	return true;
}

static void	drop_new_shape(t_game *game, t_shape *current)
{
	t_shape new_shape = duplicate_shape(&Tetriminoes[rand() % 7]);
	new_shape.col = rand() % (C - new_shape.width + 1);
	new_shape.row = 0;
	destroy_shape(current);
	*current = new_shape;
	if (!check_placed(game->board, current)) {
		game->on = false;
	}
}

static void	place_shape_to_board(t_board board, const t_shape* shape)
{
	for (int i = 0; i < shape->width; i++)
	{
		for (int j = 0; j < shape->width; j++)
		{
			board[shape->row + i][shape->col + j] |= shape->array[i][j];
		}
	}
}

static int	remove_filled_lines(t_game *game)
{
	int count = 0;
	for (int n = 0; n < R; n++)
	{
		int sum = 0;
		for (int m = 0; m < C; m++)
		{
			sum += game->board[n][m];
		}
		if (sum == C)
		{
			count++;
			int k;
			for (k = n; k >= 1; k--)
				memcpy(&game->board[k][0], &game->board[k - 1][0], sizeof(char) * C);
			memset(&game->board[k][0], 0, sizeof(char) * C);
			game->timer -= game->decrease--;
		}
	}
	return count;
}

static int	move_down_shape(t_game *game, t_shape* temp, t_shape* current)
{
	temp->row++;  //move down
	if (check_placed(game->board, temp))
	{
		current->row++;
		return 0;
	}
	else
	{
		place_shape_to_board(game->board, current);
		int removed_lines = remove_filled_lines(game);
		drop_new_shape(game, current);
		return removed_lines;
	}
}

static int	has_to_update(t_game *game)
{
	return (get_current_time() - game->updated_at) > game->timer;
}

static void	print_game(t_game *game, t_shape *current)
{
	t_board buffer = {0};
	place_shape_to_board(buffer, current);
	display_to_window(buffer, game);
}

void		game_loop(t_game *game, t_shape* current)
{
	drop_new_shape(game, current);
	print_game(game, current);
	while (game->on)
	{
		int c;
		if ((c = getch()) != ERR)
		{
			t_shape temp = duplicate_shape(current);
			switch (c)
			{
				case KEY_QUICKEN:
				{
					int removed_lines = move_down_shape(game, &temp, current);
					game->final += 100 * removed_lines;
					break;
				}
				case KEY_MOVE_RIGHT:
					temp.col++;
					if (check_placed(game->board, &temp))
						current->col++;
					break;
				case KEY_MOVE_LEFT:
					temp.col--;
					if (check_placed(game->board, &temp))
						current->col--;
					break;
				case KEY_ROTATE:
					rotate_shape(&temp);
					if (check_placed(game->board, &temp))
						rotate_shape(current);
					break;
			}
			destroy_shape(&temp);
			print_game(game, current);
		}
		if  (has_to_update(game))
		{
			t_shape temp = duplicate_shape(current);
			{
				move_down_shape(game, &temp, current);
			}
			destroy_shape(&temp);
			print_game(game, current);
			game->updated_at = get_current_time();
		}
	}
}
