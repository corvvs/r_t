#ifndef SHAPE_H
#define SHAPE_H
#include "types.h"

t_shape	duplicate_shape(const t_shape *shape);
void	destroy_shape(t_shape *shape);
void	rotate_shape(t_shape *shape);

#endif
