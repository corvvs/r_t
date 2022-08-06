#ifndef UNIT_HEADERS_H
# define UNIT_HEADERS_H
#include "types.h"

//init_game.c
void	init_game(t_game *game, t_shape* current);

//game_loop.c
suseconds_t	usec(t_timeval* t);
suseconds_t	diff_time(t_timeval *t0, t_timeval *t1);
int		move_down_shape(t_game *game, t_shape* temp, t_shape* current);
int		has_to_update(t_game *game);
void	print_game(t_game *game, t_shape *current);
void	game_loop(t_game *game, t_shape* current);
void	display_to_window(t_board buffer, t_game *game);

//tetris.c
void	destroy_shape(t_shape *shape);
t_shape	duplicate_shape(const t_shape *shape);
void	rotate_shape(t_shape *shape);
bool	check_placed(t_board board, const t_shape *shape);
void	place_shape_to_board(t_board board, const t_shape* shape);
void	drop_new_shape(t_game *game, t_shape *current);
int		remove_filled_lines(t_game *game);

//finish_game.c
static void     display_result(t_game *game);
void	finish_game(t_game *game, t_shape* current);

//display.c
void	create_window();
void	display_to_window(t_board buffer, t_game *game);
void	destroy_window();

#endif