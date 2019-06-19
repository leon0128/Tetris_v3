#pragma once

#include "sprite_actor.hpp"

class GameBoard : public SpriteActor
{
public:
    GameBoard(class Game* game, int order);

    // いくつかの関数の実行
    void update(int currentFrame) override;

private:
    // mActiveTetrominoがnullptrの場合、新しいミノを作成
    void pickTetromino();

    // ブロックを格納する配列
    std::vector<std::array<class Block*, GAMEBOARD_PARALLEL>> mGameState;
    // 操作するTetromino
    class Tetromino* mActiveTetrominio;

    // 7種のミノのタイプを一時的に格納する配列
    std::vector<EType> mPendingTetromino;
};