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
    // 計算で使用する関数
    // 最高点が一番低い位置の取得
    static int getLeastHeight(std::vector<std::array<bool, GAMEBOARD_PARALLEL>>);
    // GameStateを引数の値を元に更新
    static auto updateGameState(std::vector<std::array<bool, GAMEBOARD_PARALLEL>> argGameState,
                                EType type,
                                int direction,
                                int coordinate);
    // 初期値のテトロミノの座標を格納した配列を返す
    std::array<Vector2, 4> getInitializeTetrominoCoordinate(EType type);
    // テトロミノの回転後の座標を返す
    std::array<Vector2, 4> getRotationTetrominoCoordinate(std::array<Vector2, 4> tetromino,
                                                          int direction);

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