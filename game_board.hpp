#pragma once

#include "sprite_actor.hpp"

// 固定されるまでの時間と落下時間
// === FIX_COUNT >= DROP_COUNT ===
const int FIX_COUNT = 60;
const int DROP_COUNT = 60;
// 長押し判定までのフレームと移動速度
const int KEEPED_FRAME = 15;
const int MOVE_FRAME = 5;

class GameBoard : public SpriteActor
{
public:
    GameBoard(class Game* game, int order);

    // いくつかの関数の実行
    void update() override;

    // mGameStateの取得
    auto getGameState(){return mGameState;}

private:
    // mActiveTetrominoがnullptrの場合、新しいミノを作成
    void pickTetromino();
    // ミノの並行指導と回転移動を処理する
    void updateActiveTetromino();
    // ミノの固定、mGameStateの更新
    void updateGameState();
    // mGameStateに格納されているミノの位置を更新
    void updateBlockPosition();

    // GameBoardが保持する各オブジェクトの初期化
    void initializeHoldBoard();

    // ブロックを格納する配列
    std::vector<std::array<class Block*, GAMEBOARD_PARALLEL>> mGameState;
    // 操作するTetromino
    class Tetromino* mActiveTetrominio;
    // 7種のミノのタイプを一時的に格納する配列
    std::vector<EType> mPendingTetromino;

    // Game::mKeyboardStateを格納する配列
    std::vector<int> mKeyboardState;

    // hold用のSideBoardの設定
    class SideBoard* mHoldBoard;
};