#pragma once

#include "actor.hpp"

class AI : public Actor
{
public:
    // 結果を格納する構造体
    struct Result
    {
        unsigned int direction;  // 向き
        int coordinate;          // 中心のx座標
        bool isHoled;            // ホールドするかどうか
    };

    // 計算に必要な要素の設定
    static void startCalculation(EType active,
                                 EType hold,
                                 std::vector<EType> next,
                                 std::vector<std::array<class Block*, GAMEBOARD_PARALLEL>> gameState);

    // 実際に計算する処理
    static void calculate();

    // メンバ変数の取得
    static bool isCalculating(){return mIsCalculating;}
    static struct Result getResult(){return mResult;}

protected:
    // 引数で入手したmGameBoardの状態
    static std::vector<std::array<bool, GAMEBOARD_PARALLEL>> mVirtualGameState;
    static EType mActiveTetromino;
    static EType mHoldTetromino;
    static std::vector<EType> mNextTetromino;

    // 計算中かどうか
    static bool mIsCalculating;
    // 計算を開始するかどうか
    static bool mIsStarted;
    // 結果を格納する構造体
    static struct Result mResult;
};