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
        enum EDirection direction; // 向き
        int coordinate;            // 中心のx座標
        bool isHoled;              // ホールドするかどうか
    };

protected:
    // 計算開始処理と計算終了処理
    void startCalculation();
    void endCalculation();

    //  所属するGameBoardクラス
    class GameBoard* mGameBoard;

    // 計算が終わったかどうか
    bool mIsCalculating;
    // 結果を格納する構造体
    struct Result mResult;
};