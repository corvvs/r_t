#ifndef DISPLAY_H
#define DISPLAY_H
#include "types.h"

void	create_window();
void	display_to_window(t_board buffer, t_game *game);
void	destroy_window();

#endif