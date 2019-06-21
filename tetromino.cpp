#include "tetromino.hpp"
#include "game_board.hpp"
#include "block.hpp"

Tetromino::Tetromino(Game* game, 
                     int order, 
                     GameBoard* gameBoard,
                     EType type):
    Actor(game, order),
    mGameBoard(gameBoard),
    mType(type),
    mDownFrame(0),
    mMoveFrame(0),
    mIsQuickDrop(false)
{
    createBlock(mType);
    mDownFrame = mGame->getFrameCount();
    mMoveFrame = mGame->getFrameCount();
}

void Tetromino::update()
{
    for(auto block : mBlock)
    {
        block->updatePosition();
    }
}

bool Tetromino::parallelMove(int direction)
{
    if(direction == 0)
    {
        return true;
    }

    storeCoordinate();

    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.x += direction;
        block->setCoordinate(temp);
    }

    if(!isCoordinateCorrect())
    {
        restoreCoordinate();
        return false;
    }
    
    return true;
}

bool Tetromino::verticalMove(int direction)
{
    if(direction == 0 &&
       mGame->getFrameCount() - mDownFrame  < DROP_COUNT)
    {
        return true;
    }

    storeCoordinate();

    // 1マス移動
    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.y -= 1;
        block->setCoordinate(temp);
    }

    if(!isCoordinateCorrect())
    {
        restoreCoordinate();
        return false;
    }
    else
    {
        // 下に下がった時のフレームの更新
        mDownFrame = mGame->getFrameCount();
        return true;
    }
}

bool Tetromino::rotationMove(int direction)
{
    if(direction == 0)
    {
        return true;
    }

    // 優先度の高いブロックを中心として処理
    for(int i = 0; i < (int)mBlock.size(); i++)
    {
        storeCoordinate();

        for(auto block : mBlock)
        {
            Vector2 target, distance;
            target = block->getCoordinate();
            distance = target - mBlock[i]->getCoordinate();

            target.x += direction * 
                        distance.y -
                        distance.x;
            target.y += -1 *
                        direction *
                        distance.x -
                        distance.y;
            
            block->setCoordinate(target);
        }
        
        if(isCoordinateCorrect())
        {
            return true;
        }
        else
        {
            // 変更後の座標を1段下げて再計算
            Vector2 temp;
            for(auto block : mBlock)
            {
                temp = block->getCoordinate();
                temp.y -= 1;
                block->setCoordinate(temp);
            }
            if(isCoordinateCorrect())
            {
                return true;
            }
            else
            {
                restoreCoordinate();
            }
        }
    }

    return false;
}

void Tetromino::quickDrop(bool isQuickDrop)
{
    if(!isQuickDrop)
    {
        mIsQuickDrop = false;
        return;
    }

    // 移動が失敗するまで下方向に移動
    bool isCorrectMove = true;
    while(isCorrectMove)
    {
        isCorrectMove = verticalMove((int)-1);
    }
    mIsQuickDrop = true;
}

bool Tetromino::isCoordinateCorrect()
{
    auto gameState = mGameBoard->getGameState();
    Vector2 coordinate;

    for(auto block : mBlock)
    {
        coordinate = block->getCoordinate();
        
        if(coordinate.x < 0 || 
           coordinate.x > GAMEBOARD_PARALLEL - 1 ||
           coordinate.y < 0)
        {
            return false;
        }

        if(gameState[coordinate.y][coordinate.x])
        {
            return false;
        }
    }

    mMoveFrame = mGame->getFrameCount();
    return true;
}

void Tetromino::storeCoordinate()
{
    mBackup.clear();
    for(auto block : mBlock)
    {
        mBackup.push_back(block->getCoordinate());
    }
}

void Tetromino::restoreCoordinate()
{
    for(int i = 0; i < (int)mBlock.size(); i++)
    {
        mBlock[i]->setCoordinate(mBackup[i]);
    }
}

void Tetromino::createBlock(EType type)
{
    std::array<Vector2, 4> tempCoordinate;
    SDL_Texture* texture;

    // 作成するブロックの各座標を設定
    switch(type)
    {
        case(I):
            tempCoordinate[0].set(4, 19);
            tempCoordinate[1].set(5, 19);
            tempCoordinate[2].set(3, 19);
            tempCoordinate[3].set(6, 19);
            texture = mGame->getTexture("image/blocks/i.png");
            break;

        case(O):
            tempCoordinate[0].set(4, 19);
            tempCoordinate[1].set(4, 18);
            tempCoordinate[2].set(5, 18);
            tempCoordinate[3].set(5, 19);
            texture = mGame->getTexture("image/blocks/o.png");
            break;

        case(T):
            tempCoordinate[0].set(4, 18);
            tempCoordinate[1].set(4, 19);
            tempCoordinate[2].set(3, 18);
            tempCoordinate[3].set(5, 18);
            texture = mGame->getTexture("image/blocks/t.png");
            break;    

        case(L):
            tempCoordinate[0].set(4, 18);
            tempCoordinate[1].set(5, 18);
            tempCoordinate[2].set(3, 18);
            tempCoordinate[3].set(5, 19);
            texture = mGame->getTexture("image/blocks/l.png");
            break;

        case(J):
            tempCoordinate[0].set(4, 18);
            tempCoordinate[1].set(3, 18);
            tempCoordinate[2].set(5, 18);
            tempCoordinate[3].set(3, 19);
            texture = mGame->getTexture("image/blocks/j.png");
            break; 

        case(S):
            tempCoordinate[0].set(4, 18);
            tempCoordinate[1].set(4, 19);
            tempCoordinate[2].set(3, 18);
            tempCoordinate[3].set(5, 19);
            texture = mGame->getTexture("image/blocks/s.png");
            break; 

        case(Z):
            tempCoordinate[0].set(4, 18);
            tempCoordinate[1].set(4, 19);
            tempCoordinate[2].set(5, 18);
            tempCoordinate[3].set(3, 19);
            texture = mGame->getTexture("image/blocks/z.png");
            break; 
    }

    for(auto coordinate : tempCoordinate)
    {
        Block* block = new Block(mGame, 100, mGameBoard, coordinate);
        block->setTexture(texture);
        block->setScale(static_cast<float>(BLOCK_SIZE) / 60.0f);
        mBlock.push_back(block);
        mBackup.push_back(coordinate);
    }
}