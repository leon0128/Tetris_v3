#include "game_board.hpp"
#include "tetromino.hpp"
#include "block.hpp"

GameBoard::GameBoard(Game* game, int order):
    SpriteActor(game, order),
    mActiveTetrominio(nullptr)
{
    // 自身の位置とテクスチャの設定
    mPosition.set(WINDOW_WIDTH * 3 / 8, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/game_board.png"));

    // mGameStateの設定
    for(int i = 0; i < GAMEBOARD_VERTICAL; i++)
    {
        std::array<class Block*, GAMEBOARD_PARALLEL> line = {nullptr};
        mGameState.push_back(line);
    }
}

void GameBoard::update()
{
    pickTetromino();
    updateActiveTetromino();
    updateGameState();
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

void GameBoard::updateActiveTetromino()
{
    mActiveTetrominio->parallelMove(0);
    mActiveTetrominio->verticalMove(0);
    mActiveTetrominio->rotationMove(0);
}

void GameBoard::updateGameState()
{
    if(mGame->getFrameCount() - mActiveTetrominio->getMoveFrame() < FIX_COUNT)
    {
        return ;
    }

    // ブロックの固定
    Vector2 temp;
    for(auto block : mActiveTetrominio->getBlock())
    {
        temp = block->getCoordinate();
        mGameState[temp.y][temp.x] = block;
    }
    // mActiveTetrominoの削除
    mGame->deletedActor(mActiveTetrominio);
    mActiveTetrominio = nullptr;

    // ブロックで埋め尽くされた列の探索
    std::vector<int> filledLine;
    for(int y = 0; y < GAMEBOARD_VERTICAL; y++)
    {
        for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
        {
            if(!mGameState[y][x])
            {
                break;
            }
            else if(x == GAMEBOARD_PARALLEL - 1)
            {
                filledLine.push_back(y);
            }
        }
    }

    // 埋め尽くされた列の削除と新しい列の作成
    while(!filledLine.empty())
    {
        // 埋め尽くされた列の中で一番下の列の削除
        for(auto block : mGameState[*filledLine.begin()])
        {
            mGame->deletedActor(block);
        }
        mGameState.erase(mGameState.begin() + *(filledLine.begin()));

        // 削除した列より上のブロックのy座標を1下げる
        for(int y = *filledLine.begin();
            y < (int)mGameState.size();
            y++)
        {
            for(auto block : mGameState[y])
            {
                if(block)
                {
                    temp = block->getCoordinate();
                    temp.y -= 1;
                    block->setCoordinate(temp);
                }
            }
        }

        // 新しい列の追加
        std::array<Block*, 10> line = {nullptr};
        mGameState.push_back(line);

        // filledLineの値を全て１ずつ減らし、使用した値の削除
        for(int i = 0; i < (int)filledLine.size(); i++)
        {
            filledLine[i] -= 1;
        }
        filledLine.erase(filledLine.begin());
    }
}