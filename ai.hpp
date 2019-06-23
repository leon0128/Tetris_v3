#pragma once

#include "actor.hpp"

class AI : public Actor
{
public:
    AI(class Game* game,
       int order,
       class GameBoard* gameBoard);

    // 結果を格納する構造体
    struct Result
    {
        unsigned int direction;  // 向き
        int coordinate;          // 中心のx座標
        bool isHoled;            // ホールドするかどうか
    };

    // 計算処理
    // 各々で実装
    virtual void calculate(){}

    void startCalculation(EType active,
                          EType hold,
                          std::vector<EType> next,
                          std::vector<std::array<class Block*, GAMEBOARD_PARALLEL>> gameState);

    // メンバ変数の取得
    bool isCalculating(){return mIsCalculating;}
    struct Result getResult(){return mResult;}

protected:

    //  所属するGameBoardクラス
    class GameBoard* mGameBoard;
    // 引数で入手したmGameBoardの状態
    std::vector<std::array<bool, GAMEBOARD_PARALLEL>> mVirtualGameState;
    EType mActiveTetromino;
    EType mHoldTetromino;
    std::vector<EType> mNextTetromino;

    // 計算が終わったかどうか
    bool mIsCalculating;
    // 結果を格納する構造体
    struct Result mResult;
};