#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H
#include "types.h"

int		get_char();
void	create_window();
void	display_to_window(t_board buffer, t_game *game);
void	destroy_window();

#endif
