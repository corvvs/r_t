#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define R 20
#define C 15
#define T 1
#define F 0

typedef char t_board[R][C];
t_board GameBoard = {0};
int final = 0;
char GameOn = T;
suseconds_t timer = 400000;
int decrease = 1000;

typedef struct s_shape {
	char **array;
	int width, row, col;
} t_shape;
t_shape current;

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

int check_placed(t_board board, const t_shape *shape)
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
					return F;
				
			}
			else if (board[shape->row + i][shape->col + j] && array[i][j])
				return F;
		}
	}
	return T;
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

void print_game(t_board board)
{
	t_board Buffer = {0};
	place_shape_to_board(Buffer, &current);
	clear();
	for (int i = 0; i < C - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printw("%c ", (board[i][j] + Buffer[i][j])? '#': '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", final);
}

struct timeval before_now, now;
int hasToUpdate()
{
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > timer;
}

void set_timeout(int time)
{
	timeout(time);
}

int remove_filled_lines(t_board board)
{
	int count = 0;
	for (int n = 0; n < R; n++)
	{
		int sum = 0;
		for (int m = 0; m < C; m++)
		{
			sum += board[n][m];
		}
		if (sum == C)
		{
			count++;
			int k;
			for (k = n; k >= 1; k--)
				memcpy(&board[k][0], &board[k - 1][0], sizeof(char) * C);
			memset(&board[k][0], 0, sizeof(char) * C);
			timer -= decrease--;
		}
	}
	return count;
}

void drop_new_shape(t_board board, t_shape *current)
{
	t_shape new_shape = duplicate_shape(&Tetriminoes[rand() % 7]);
	new_shape.col = rand() % (C - new_shape.width + 1);
	new_shape.row = 0;
	destroy_shape(current);
	*current = new_shape;
	if (!check_placed(board, current)) {
		GameOn = F;
	}
}

int move_down_shape(t_board board, t_shape* temp, t_shape* current)
{
	temp->row++;  //move down
	if (check_placed(board, temp))
	{
		current->row++;
		return 0;
	}
	else
	{
		place_shape_to_board(board, current);
		int removed_lines = remove_filled_lines(board);
		drop_new_shape(board, current);
		return removed_lines;
	}
}

void init_game(t_board board)
{
	srand(time(0));
	final = 0;
	initscr();
	gettimeofday(&before_now, NULL);
	set_timeout(1);
	t_shape new_shape = duplicate_shape(&Tetriminoes[rand() % 7]);
	new_shape.col = rand() % (C - new_shape.width + 1);
	new_shape.row = 0;
	destroy_shape(&current);
	current = new_shape;
	if (!check_placed(board, &current))
	{
		GameOn = F;
	}
}

void game_loop(t_board board, t_shape* current)
{
	print_game(board);
	while (GameOn)
	{
		int c;
		if ((c = getch()) != ERR)
		{
			t_shape temp = duplicate_shape(current);
			switch (c)
			{
				case 's':
				{
					int removed_lines = move_down_shape(board, &temp, current);
					final += 100 * removed_lines;
					break;
				}
				case 'd':
					temp.col++;
					if (check_placed(board, &temp))
						current->col++;
					break;
				case 'a':
					temp.col--;
					if (check_placed(board, &temp))
						current->col--;
					break;
				case 'w':
					rotate_shape(&temp);
					if (check_placed(board, &temp))
						rotate_shape(current);
					break;
			}
			destroy_shape(&temp);
			print_game(board);
		}
		gettimeofday(&now, NULL);
		if (hasToUpdate())
		{
			t_shape temp = duplicate_shape(current);
			{
				move_down_shape(board, &temp, current);
			}
			destroy_shape(&temp);
			print_game(board);
			gettimeofday(&before_now, NULL);
		}
	}
}

void finish_game(t_board board)
{
	endwin();
	destroy_shape(&current);
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printf("%c ", board[i][j] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", final);
}

int main()
{
	init_game(GameBoard);
	game_loop(GameBoard, &current);
	finish_game(GameBoard);
	return 0;
}
