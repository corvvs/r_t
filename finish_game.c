#include "tetris.h"

void	destroy_window()
{
	endwin();
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
