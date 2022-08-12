#ifndef TETRIS_TYPES_H
#define TETRIS_TYPES_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

// ゲームパラメータ
#define R 20
#define C 15
#define INITIAL_TURN_DURATION 400000
#define INITIAL_TURN_DECREASEMENT 1000

// キーコンフィグ
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
	// ゲームのスコア
	int			score;
	// ゲームターンの長さ
	// ゲームターンの開始からこの長さだけ経過すると,
	// 今浮いているブロックが強制的に1ブロック落とされる.
	suseconds_t	turn_duration;
	// 1ライン消した時のゲームターン長の減少量
	// これ自体も少しずつ減っていくことで難易度曲線を調整している(んでしょうねきっと)
	suseconds_t	duration_decreasement;
	// ゲームターンの開始時刻
	suseconds_t	turn_started_at;
}	t_game;

#endif
