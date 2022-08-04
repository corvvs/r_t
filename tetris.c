#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define R 20
#define C 15
#define KEY_QUICKEN		's'
#define KEY_MOVE_RIGHT	'd'
#define KEY_MOVE_LEFT	'a'
#define KEY_ROTATE		'w'

typedef char t_board[R][C];

typedef struct s_shape {
	char **array;
	int width, row, col;
} t_shape;

typedef struct s_game
{
	t_board		board;
	suseconds_t	timer;
	bool		on;
	int			final;
	int			decrease;
}	t_game;

const t_shape Tetriminoes[7] = {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};

t_shape duplicate_shape(const t_shape *shape)
{
	t_shape	new_shape = *shape;
	char	**copyshape = shape->array;
	new_shape.array = (char**)malloc(new_shape.width * sizeof(char*));
	for (int i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char*)malloc(new_shape.width * sizeof(char));
		memcpy(&new_shape.array[i][0], &copyshape[i][0], new_shape.width * sizeof(char));
	}
	return new_shape;
}

void destroy_shape(t_shape *shape)
{
	for (int i = 0; i < shape->width; i++)
	{
		free(shape->array[i]);
	}
	free(shape->array);
}

bool check_placed(t_board board, const t_shape *shape)
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

void rotate_shape(t_shape *shape)
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

void place_shape_to_board(t_board board, const t_shape* shape)
{
	for (int i = 0; i < shape->width; i++)
	{
		for (int j = 0; j < shape->width; j++)
		{
			board[shape->row + i][shape->col + j] |= shape->array[i][j];
		}
	}
}

void print_game(t_game *game, t_shape *current)
{
	t_board Buffer = {0};
	place_shape_to_board(Buffer, current);
	clear();
	for (int i = 0; i < C - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printw("%c ", (game->board[i][j] + Buffer[i][j])? '#': '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->final);
}

suseconds_t usec(struct timeval* t)
{
	return (suseconds_t)(t->tv_sec * 1000000 + t->tv_usec);
}

struct timeval updated_at, now;
int hasToUpdate(t_game *game)
{
	return (usec(&now) - usec(&updated_at)) > game->timer;
}

void set_timeout(int time)
{
	timeout(time);
}

int remove_filled_lines(t_game *game)
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

void drop_new_shape(t_game *game, t_shape *current)
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

int move_down_shape(t_game *game, t_shape* temp, t_shape* current)
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

void init_game(t_game *game, t_shape* current)
{
	srand(time(0));
	initscr();
	gettimeofday(&updated_at, NULL);
	set_timeout(1);
	drop_new_shape(game, current);
}

void game_loop(t_game *game, t_shape* current)
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
		gettimeofday(&now, NULL);
		if (hasToUpdate(game))
		{
			t_shape temp = duplicate_shape(current);
			{
				move_down_shape(game, &temp, current);
			}
			destroy_shape(&temp);
			print_game(game, current);
			gettimeofday(&updated_at, NULL);
		}
	}
}

void finish_game(t_game *game, t_shape* current)
{
	endwin();
	destroy_shape(current);
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

int main()
{
	t_game	game = {
		.board = {0},
		.timer = 400000,
		.on = true,
		.final = 0,
		.decrease = 1000,
	};
	t_shape	current = {0};
	init_game(&game, &current);
	game_loop(&game, &current);
	finish_game(&game, &current);
	return 0;
}
