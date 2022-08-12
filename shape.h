#ifndef TETRIS_SHAPE_H
#define TETRIS_SHAPE_H
#include "types.h"

t_shape	create_shape();
t_shape	duplicate_shape(const t_shape *shape);
void	destroy_shape(t_shape *shape);
void	rotate_shape_clockwise(t_shape *shape);

#endif
