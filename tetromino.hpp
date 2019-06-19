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
    void update() override;

    // 平行移動と回転移動
    // parallelMove(),vertilcalMove()は、引数の値で移動の向き
    // rotationMove()は-1で半時計回り、+1で時計回り
    void parallelMove(int direction);
    void verticalMove(int direction);
    void rotationMove(int direction);

private:
    // mBlockの各ブロックが正しい位置にあるか
    bool isCoordinateCorrect();
    // mBlockの座標をmBackupに変更
    void restoreCoordinate();
    
    // 引数のタイプに応じてブロックを作成
    void createBlock(EType type);

    // 自身の所属するGameBoard
    class GameBoard* mGameBoard;
    // ミノの種類
    enum EType mType;
    // ブロックを格納する配列
    std::vector<class Block*> mBlock;
    // 移動前のブロックの座標を格納
    std::vector<class Vector2> mBackup;

    // 最後に下に移動した時のFrame
    int mDownFrame;
};