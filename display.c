#include "display.h"

void	create_window()
{
	initscr();
	timeout(1);
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

void	destroy_window()
{
	endwin();
}