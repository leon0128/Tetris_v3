#pragma once

#include "actor.hpp"

class Tetromino : public Actor
{
public:
    // 第三引数は所属するゲームボードクラス
    Tetromino(class Game* game,
              int order,
              class GameBoard* gameBoard,
              EType type);

private:
    // 引数のタイプに応じてブロックを作成
    void createBlock(EType type);

    // 自身の所属するGameBoard
    class GameBoard* mGameBoard;
    // ミノの種類
    enum EType mType;
    // ブロックを格納する配列
    std::vector<class Block*> mBlock;
};