#include <string.h>
#include <stdlib.h>
#include "types.h"

// テトロミノ7種(SZTLJOI)
const t_shape Tetrominoes[7] = {
	{
		// S
		.array = (char *[]){
			(char []){0,1,1},
			(char []){1,1,0},
			(char []){0,0,0},
		},
		.width = 3,
	},
	{
		// Z
		.array = (char *[]){
			(char []){1,1,0},
			(char []){0,1,1},
			(char []){0,0,0},
		},
		.width = 3,
	},
	{
		// T
		.array = (char *[]){
			(char []){0,1,0},
			(char []){1,1,1},
			(char []){0,0,0},
		},
		.width = 3,
	},
	{
		// L 
		.array = (char *[]){
			(char []){0,0,1},
			(char []){1,1,1},
			(char []){0,0,0},
		},
		.width = 3,
	},
	{
		// J
		.array = (char *[]){
			(char []){1,0,0},
			(char []){1,1,1},
			(char []){0,0,0}
		},
		.width = 3,
	},
	{
		// O
		.array = (char *[]){
			(char []){1,1},
			(char []){1,1},
		},
		.width = 2,
	},
	{
		// I
		.array = (char *[]){
			(char []){0,0,0,0},
			(char []){1,1,1,1},
			(char []){0,0,0,0},
			(char []){0,0,0,0}
		},
		.width = 4,
	}
};

t_shape	duplicate_shape(const t_shape *shape)
{
	t_shape	new_shape = *shape;
	char	**copyshape = shape->array;
	new_shape.array = (char **)malloc(new_shape.width * sizeof(char*));
	for (int i = 0; i < new_shape.width; i++) {
		new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
		memcpy(new_shape.array[i], copyshape[i], new_shape.width * sizeof(char));
	}
	return new_shape;
}

t_shape	create_shape()
{
	t_shape new_shape = duplicate_shape(&Tetrominoes[rand() % 7]);
	new_shape.col = rand() % (C - new_shape.width + 1);
	new_shape.row = 0;
	return new_shape;
}

void	destroy_shape(t_shape *shape)
{
	for (int i = 0; i < shape->width; i++) {
		free(shape->array[i]);
	}
	free(shape->array);
}

void	rotate_shape_clockwise(t_shape *shape)
{
	t_shape temp = duplicate_shape(shape);
	int width = shape->width;
	for (int i = 0; i < width; i++) {
		for (int j = 0, k = width - 1; j < width; j++, k--) {
			shape->array[i][j] = temp.array[k][i];
		}
	}
	destroy_shape(&temp);
}
