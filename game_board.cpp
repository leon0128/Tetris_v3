#include "game_board.hpp"
#include "tetromino.hpp"
#include "block.hpp"
#include "side_board.hpp"

GameBoard::GameBoard(Game* game, int order):
    SpriteActor(game, order),
    mActiveTetrominio(nullptr),
    mHoldBoard(nullptr)
{
    // 自身の位置とテクスチャの設定
    mPosition.set(WINDOW_WIDTH * 3 / 8, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/game_board.png"));
    setClear(0.65f);

    // mGameStateの設定
    for(int i = 0; i < GAMEBOARD_VERTICAL; i++)
    {
        std::array<class Block*, GAMEBOARD_PARALLEL> line = {nullptr};
        mGameState.push_back(line);
    }

    // HoldBoardの設定
    initializeHoldBoard();
}

void GameBoard::update()
{
    // キーボードの状態の更新
    mKeyboardState = mGame->getKeyboardState();
    
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
    int parallel = 0, vertical = 0, rotation = 0;
    bool isQuickDrop = false;
    if((mKeyboardState[SDL_SCANCODE_A] == 1) ||
       (mKeyboardState[SDL_SCANCODE_A] >= KEEPED_FRAME && 
       (mKeyboardState[SDL_SCANCODE_A] - KEEPED_FRAME) % MOVE_FRAME == 0))
        parallel --;
    if((mKeyboardState[SDL_SCANCODE_D] == 1) ||
       (mKeyboardState[SDL_SCANCODE_D] >= KEEPED_FRAME && 
       (mKeyboardState[SDL_SCANCODE_D] - KEEPED_FRAME) % MOVE_FRAME == 0))
        parallel ++;
    if((mKeyboardState[SDL_SCANCODE_S] == 1) ||
       (mKeyboardState[SDL_SCANCODE_S] >= KEEPED_FRAME && 
       (mKeyboardState[SDL_SCANCODE_S] - KEEPED_FRAME) % MOVE_FRAME == 0))
        vertical --;
    if(mKeyboardState[SDL_SCANCODE_J] == 1)
        rotation --;
    if(mKeyboardState[SDL_SCANCODE_K] == 1)
        rotation ++;
    if(mKeyboardState[SDL_SCANCODE_W] == 1)
        isQuickDrop = true;
    
    
    mActiveTetrominio->parallelMove(parallel);
    mActiveTetrominio->verticalMove(vertical);
    mActiveTetrominio->rotationMove(rotation);
    mActiveTetrominio->quickDrop(isQuickDrop);
    mActiveTetrominio->updateShadow();
}

void GameBoard::updateGameState()
{
    if(mGame->getFrameCount() - mActiveTetrominio->getMoveFrame() < FIX_COUNT && 
       !mActiveTetrominio->isQuickDrop())
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
    delete mActiveTetrominio;
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
            delete block;
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
    
    // mGameBoardに格納されているブロックの更新
    updateBlockPosition();
}

void GameBoard::updateBlockPosition()
{
    for(int y = 0; y < (int)mGameState.size(); y++)
    {
        for(int x = 0; x < (int)mGameState[y].size(); x++)
        {
            if(mGameState[y][x])
            {
                mGameState[y][x]->updatePosition();
            }
        }
    }
}

void GameBoard::initializeHoldBoard()
{
    // HoldBoardの設定
    mHoldBoard = new SideBoard(mGame, 40, this);
    Vector2 position = getPosition();
    position.x -= mTextureSize.x / 2 + 100;
    position.y -= mTextureSize.y / 2 - 100;
    mHoldBoard->setPosition(position);
    mHoldBoard->updateRectangle();   
}