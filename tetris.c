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

t_shape	duplicate_shape(const t_shape *shape)
{
	t_shape	new_shape = *shape;
	char	**copyshape = shape->array;
	new_shape.array = (char **)malloc(new_shape.width * sizeof(char*));
	for (int i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
		memcpy(&new_shape.array[i][0], &copyshape[i][0], new_shape.width * sizeof(char));
	}
	return new_shape;
}

void	destroy_shape(t_shape *shape)
{
	for (int i = 0; i < shape->width; i++)
	{
		free(shape->array[i]);
	}
	free(shape->array);
}

bool	check_placed(t_board board, const t_shape *shape)
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

void	rotate_shape(t_shape *shape)
{
	t_shape temp = duplicate_shape(shape);
	int width = shape->width;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0, k = width - 1; j < width; j++, k--)
		{
			shape->array[i][j] = temp.array[k][i];
		}
	}
	destroy_shape(&temp);
}

void	place_shape_to_board(t_board board, const t_shape* shape)
{
	for (int i = 0; i < shape->width; i++)
	{
		for (int j = 0; j < shape->width; j++)
		{
			board[shape->row + i][shape->col + j] |= shape->array[i][j];
		}
	}
}

suseconds_t	usec(t_timeval* t)
{
	return (suseconds_t)(t->tv_sec * 1000000 + t->tv_usec);
}

suseconds_t	diff_time(t_timeval *t0, t_timeval *t1)
{
	return (usec(t1) - usec(t0));
}

int		remove_filled_lines(t_game *game)
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

void	drop_new_shape(t_game *game, t_shape *current)
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
	gettimeofday(&game->updated_at, NULL);
	drop_new_shape(game, current);
	create_window();
}

void	display_result(t_game *game)
{
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printf("%c ", game->board[i][j] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", game->final);
}

void	finish_game(t_game *game, t_shape* current)
{
	destroy_window();
	destroy_shape(current);
	display_result(game);
}

int		main()
{
	t_game	game;
	t_shape	current;

	init_game(&game, &current);
	game_loop(&game, &current);
	finish_game(&game, &current);
	return 0;
}