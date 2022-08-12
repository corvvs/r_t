#ifndef TETRIS_KEY_ACTION_H
#define TETRIS_KEY_ACTION_H
#include "types.h"

void	place_shape_to_board(t_board board, const t_shape* shape);
void	drop_new_shape(t_game *game, t_shape *current);

void	perform_quicken(t_game *game, t_shape *current);
void	perform_move_right(t_game *game, t_shape *current);
void	perform_move_left(t_game *game, t_shape *current);
void	perform_rotate_clockwise(t_game *game, t_shape *current);


#endif
