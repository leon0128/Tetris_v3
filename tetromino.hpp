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

    // 影に使用したブロックの削除
    virtual ~Tetromino();

    // 各ブロックのmCoordinateを元にmPositionを更新
    void update() override;

    // 平行移動と回転移動
    // parallelMove(),vertilcalMove()は、引数の値で移動の向き
    // rotationMove()は-1で半時計回り、+1で時計回り
    // 戻り値は移動が成功したか
    bool parallelMove(int direction);
    bool verticalMove(int direction);
    bool rotationMove(int direction);
    void quickDrop(bool isQuickDrop);
    void updateShadow();

    // mBlockの取得
    auto getBlock(){return mBlock;}
    // mMoveFrameの取得
    int getMoveFrame(){return mMoveFrame;}
    // mIsQuickDropの取得
    bool isQuickDrop(){return mIsQuickDrop;}
    // mTypeの取得
    EType getType(){return mType;}

private:
    // mBlockの各ブロックが正しい位置にあるか
    bool isCoordinateCorrect(std::vector<class Block*>);
    // mBackupに値を格納
    void storeCoordinate(std::vector<class Block*>);
    // mBlockの座標をmBackupに変更
    void restoreCoordinate(std::vector<class Block*>);
    
    // 引数のタイプに応じてブロックを作成
    void createBlock(EType type);

    // 自身の所属するGameBoard
    class GameBoard* mGameBoard;
    // ミノの種類
    enum EType mType;
    // ブロックを格納する配列
    std::vector<class Block*> mBlock;
    // shadowを格納する配列
    std::vector<class Block*> mShadowBlock;
    // 移動前のブロックの座標を格納
    std::vector<class Vector2> mBackup;

    // 最後に下に移動した時のFrame
    int mDownFrame;
    // 最後に座標を移動した時のFrame
    int mMoveFrame;
    // クイックドロップしたかどうか
    bool mIsQuickDrop;
};