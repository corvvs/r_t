#include <ncurses.h>
#include "types.h"
#define KEY_INPUT_WAIT_TIME_MS 1

// ncurses 関数はすべてこのファイルに押し込まれている

int		get_char()
{
	return getch();
}

void	create_window()
{
	initscr();
	// getch を実行してから「一定時間」待つと, キー入力がなくても getch が返ってくるようにする.
	// 「一定時間」は timeout の引数.
	timeout(KEY_INPUT_WAIT_TIME_MS);
}

void	display_to_window(t_board buffer, t_game *game)
{
	clear();
	for (int i = 0; i < C - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			printw("%c ", (game->board[i][j] + buffer[i][j]) ? '#' : '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->score);
}

void	destroy_window()
{
	endwin();
}
