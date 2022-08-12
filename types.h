#ifndef TETRIS_TYPES_H
#define TETRIS_TYPES_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

#define R 20
#define C 15
#define KEY_QUICKEN		's'
#define KEY_MOVE_RIGHT	'd'
#define KEY_MOVE_LEFT	'a'
#define KEY_ROTATE		'w'

typedef char t_board[R][C];

typedef struct	s_shape {
	char **array;
	int width, row, col;
}	t_shape;

typedef struct	s_game
{
	t_board		board;
	// ゲームが継続中かどうか
	bool		game_on;
	int			score;
	suseconds_t	turn_duration;
	suseconds_t	duration_decreasement;
	suseconds_t	turn_started_at;
}	t_game;

#endif
