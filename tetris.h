#ifndef TETRIS_H
#define TETRIS_H

// 外部公開用

#include "types.h"

// ゲームパラメータの初期化
void	init_game(t_game *game, t_shape* current);
// ゲーム本体(=ユーザのコントロールを受け付ける部分)の実行
void	game_loop(t_game *game, t_shape* current);
// ゲームの後始末
void	finish_game(t_game *game, t_shape* current);

#endif
