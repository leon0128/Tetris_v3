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

    // 各ブロックのmCoordinateを元にmPositionを更新
    void update(int currentFrame) override;

    // 平行移動と回転移動
    // parallelMove(),vertilcalMove()は、引数の値分座標を移動
    // rotationMove()は-1で半時計回り、+1で時計回り
    void parallelMove(int displacement);
    void verticalMove(int displacement);
    void rotationMove(int direction);

private:
    // 引数のタイプに応じてブロックを作成
    void createBlock(EType type);

    // 自身の所属するGameBoard
    class GameBoard* mGameBoard;
    // ミノの種類
    enum EType mType;
    // ブロックを格納する配列
    std::vector<class Block*> mBlock;

    // 最後に下に移動した時のFrame
    int mDownFrame;
};