#pragma once

#include "actor.hpp"

class NPC : public Actor
{
public:
    // GameBoardの状態を簡略化したもののエイリアス宣言
    using VirtualGameState = std::vector<std::array<bool, GAMEBOARD_PARALLEL>>;

    // 結果を格納する構造体
    struct Result
    {
        unsigned int direction;  // 向き
        int coordinate;          // 中心のx座標
        bool isHoled;            // ホールドするかどうか
    };

    // 詳細な結果を格納する構造体
    struct DetailResult
    {
        Result result;
        int empty;
        int maxHeight;
        double dispersion;
    };

    // 計算に必要な要素の設定
    // その後, NPC::calculate()の呼び出し
    static void startCalculation(EType active,
                                 EType hold,
                                 std::vector<EType> next,
                                 std::vector<std::array<class Block*, GAMEBOARD_PARALLEL>> gameState);

    // メンバ変数の取得
    static bool isCalculating() {return mIsCalculating;}
    static struct Result getResult() {return mResult;}

private:
    // 計算し、結果を格納したResult構造体を返す
    static void calculate();

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
    // gameStateの最も高い位置に存在するブロックの座標を返す
    static int getMaxHeight(VirtualGameState gameState);
    // gameStateの最も低い位置に存在するブロックの座標を返す
    static Vector2 getMinHeight(VirtualGameState gameState);
    // 空白の数を返す
    static int getEmptyNumber(VirtualGameState gameState);
    // 各x座標の最大値の分散
    static double getDispersion(VirtualGameState gameState);

    // mDetailResultVectorの中でemptyが最小のもの以外の削除
    static void deleteNonMinimumEmpty();
    // mDetailResultVectorの中でheightが最小のもの以外の削除
    static void deleteNonMinimumHeight();
    // mDetailResultVectorの中でdispersionが最小のもの以外の削除
    static void deleteNonMinimumDispersion();

    // 引数のGameStateno状態をコンソールに出力する
    static void printVirtualGameState(VirtualGameState gameState);

    // 引数で入手したmGameBoardの状態
    static VirtualGameState mVirtualGameState;
    static EType mActiveTetromino;
    static EType mHoldTetromino;
    static std::vector<EType> mNextTetromino;

    // 計算中かどうか
    static bool mIsCalculating;
    // 結果を格納する構造体
    static struct Result mResult;
    // 詳細な結果を纏める配列
    std::vector<struct DetailResult> mDetailResultVector;
};