#include "game_board.hpp"
#include "tetromino.hpp"

GameBoard::GameBoard(Game* game, int order):
    SpriteActor(game, order),
    mActiveTetrominio(nullptr)
{
    // 自身の位置とテクスチャの設定
    mPosition = new Vector2(WINDOW_WIDTH * 3 / 8, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/game_board.png"));

    // mGameStateの設定
    for(int i = 0; i < GAMEBOARD_VERTICAL; i++)
    {
        std::array<class Block*, GAMEBOARD_PARALLEL> line = {nullptr};
        mGameState.push_back(line);
    }
}

void GameBoard::update(int cuurentFrame)
{
    pickTetromino();

    mActiveTetrominio->updatePosition();
}

void GameBoard::pickTetromino()
{
    // mActiveTetrominoが存在する場合、何もせずに返す
    if(mActiveTetrominio)
    {
        return;
    }

    // mPendingTetrominoが空の場合、7種のミノのタイプを格納
    if(mPendingTetromino.empty())
    {
        mPendingTetromino.push_back(I);
        mPendingTetromino.push_back(O);
        mPendingTetromino.push_back(T);
        mPendingTetromino.push_back(L);
        mPendingTetromino.push_back(J);
        mPendingTetromino.push_back(S);
        mPendingTetromino.push_back(Z);
    }

    // 乱数を利用し、mActiveTetrominoを作成
    EType type = mPendingTetromino[rand() % mPendingTetromino.size()];
    mActiveTetrominio = new Tetromino(mGame, 50, this, type);

    // mPendingTetrominoから利用した要素の削除
    auto iterator = std::find(mPendingTetromino.begin(),
                              mPendingTetromino.end(),
                              type);
    mPendingTetromino.erase(iterator);
}