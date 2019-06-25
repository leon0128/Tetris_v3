#pragma once

#include "actor.hpp"

class AI : public Actor
{
private: 
    // GameBoardの状態を簡略化したもののエイリアス宣言
    using VirtualGameState = std::vector<std::array<bool, GAMEBOARD_PARALLEL>>;

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
    static Vector2 getMinHeight(VirtualGameState gameState);
    // GameStateを引数の値を元に更新
    static VirtualGameState updateGameState(VirtualGameState argGameState,
                                            EType type,
                                            int direction,
                                            int coordinate);
    // 初期値のテトロミノの座標を格納した配列を返す
    static std::array<Vector2, 4> getInitializeTetrominoCoordinate(EType type);
    // テトロミノの回転後の座標を返す
    static std::array<Vector2, 4> getRotationTetrominoCoordinate(std::array<Vector2, 4> tetromino,
                                                                 int direction);
    // テトロミノを平行移動させる
    static std::array<Vector2, 4> getParallelTetrominoCoordinate(std::array<Vector2, 4> tetromino,
                                                                 int coordinateX);
    // クイックドロップ後のゲームの状態を返す
    static VirtualGameState getQuickDropedGameState(VirtualGameState argGameState,
                                                    std::array<Vector2, 4> tetromino);
    // ブロックで満たされている列の削除
    static VirtualGameState deleteLine(VirtualGameState gameState);
    // 引数のx座標に被っていたらtrueを返す
    static bool isFilledX(VirtualGameState gameState,
                          int x,
                          int leastHeight);
    // gameStateの最も高い位置に存在するブロックのy座標を返す
    static int getMaxHeight(VirtualGameState gameState);
    // 空白の数を返す
    static int getEmptyNumber(VirtualGameState gameState);
    // 引数のGameStateno状態をコンソールに出力する
    static void printVirtualGameState(VirtualGameState gameState);

    // 引数で入手したmGameBoardの状態
    static VirtualGameState mVirtualGameState;
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