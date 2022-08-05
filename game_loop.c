#include "tetris.h"

suseconds_t	usec(t_timeval* t)
{
	return (suseconds_t)(t->tv_sec * 1000000 + t->tv_usec);
}

suseconds_t	diff_time(t_timeval *t0, t_timeval *t1)
{
	return (usec(t1) - usec(t0));
}

int		move_down_shape(t_game *game, t_shape* temp, t_shape* current)
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

int		has_to_update(t_game *game)
{
	t_timeval	now;
	gettimeofday(&now, NULL);
	return (diff_time(&game->updated_at, &now) > game->timer);
}

void	display_to_window(t_board buffer, t_game *game)
{
	clear();
	for (int i = 0; i < C - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printw("%c ", (game->board[i][j] + buffer[i][j])? '#': '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->final);
}

void	print_game(t_game *game, t_shape *current)
{
	t_board buffer = {0};
	place_shape_to_board(buffer, current);
	display_to_window(buffer, game);
}

void	game_loop(t_game *game, t_shape* current)
{
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
			gettimeofday(&game->updated_at, NULL);
		}
	}
}
