#pragma once

#include "sprite_actor.hpp"
#include "game_board.hpp"

class Tetromino : public SpriteActor
{
public:
    // 第三引数は所属するゲームボードクラス
    Tetromino(class Game* game,
              int order,
              class GameBoard* gameBoard);

private:
    // 自身の所属するGameBoard
    class GameBoard* mGameBoard;
};