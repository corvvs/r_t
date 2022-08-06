#ifndef TETRIS_H
#define TETRIS_H

#include "types.h"
#include "shape.h"
#include "display.h"
#include "time.h"

void	init_game(t_game *game, t_shape* current);
void	game_loop(t_game *game, t_shape* current);
void	finish_game(t_game *game, t_shape* current);

#endif
