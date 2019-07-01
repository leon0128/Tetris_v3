#include "tetromino.hpp"
#include "game_board.hpp"
#include "controller.hpp"

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
    mDownFrame = mGameBoard->getUpdateFrame();
    mMoveFrame = mGameBoard->getUpdateFrame();
}

Tetromino::~Tetromino()
{
    for(int i = 0; i < (int)mBlock.size(); i++)
    {
        delete mShadowBlock[i];
    }
}

void Tetromino::update()
{
    for(auto block : mBlock)
    {
        block->updatePosition();
    }
    for(auto block : mShadowBlock)
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

    storeCoordinate(mBlock);

    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.x += direction;
        block->setCoordinate(temp);
    }

    if(!isCoordinateCorrect(mBlock))
    {
        restoreCoordinate(mBlock);
        return false;
    }
    
    return true;
}

bool Tetromino::verticalMove(int direction)
{
    if(direction == 0 &&
       mGameBoard->getUpdateFrame() - mDownFrame  < DROP_COUNT)
    {
        return true;
    }

    storeCoordinate(mBlock);

    // 1マス移動
    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.y -= 1;
        block->setCoordinate(temp);
    }

    if(!isCoordinateCorrect(mBlock))
    {
        restoreCoordinate(mBlock);
        return false;
    }
    else
    {
        // 下に下がった時のフレームの更新
        mDownFrame = mGameBoard->getUpdateFrame();
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
        storeCoordinate(mBlock);

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
        
        if(isCoordinateCorrect(mBlock))
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
            if(isCoordinateCorrect(mBlock))
            {
                return true;
            }
            else
            {
                restoreCoordinate(mBlock);
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

void Tetromino::updateShadow()
{
    if(mMoveFrame != mGameBoard->getUpdateFrame())
    {
        return ;
    }

    // mBlockの座標をmShadowBlockにコピー
    for(int i = 0; i < (int)mBlock.size(); i++)
    {
        mShadowBlock[i]->setCoordinate(mBlock[i]->getCoordinate());
    }

    // 移動が失敗するまで下に移動
    bool isCorrectMove = true;
    while(isCorrectMove)
    {
        storeCoordinate(mShadowBlock);
        for(auto block : mShadowBlock)
        {
            Vector2 temp = block->getCoordinate();
            temp.y -= 1;
            block->setCoordinate(temp);
        }
        if(!isCoordinateCorrect(mShadowBlock))
        {
            restoreCoordinate(mShadowBlock);
            isCorrectMove = false;
        }
    }
}

bool Tetromino::isCoordinateCorrect(std::vector<Block*> blocks)
{
    auto gameState = mGameBoard->getGameState();
    Vector2 coordinate;

    for(auto block : blocks)
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

    mMoveFrame = mGameBoard->getUpdateFrame();
    return true;
}

void Tetromino::storeCoordinate(std::vector<Block*> blocks)
{
    mBackup.clear();
    for(auto block : blocks)
    {
        mBackup.push_back(block->getCoordinate());
    }
}

void Tetromino::restoreCoordinate(std::vector<Block*> blocks)
{
    for(int i = 0; i < (int)blocks.size(); i++)
    {
        blocks[i]->setCoordinate(mBackup[i]);
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
        
        case(NONE):
            SDL_Log("Can not crate Tetromino: Tetromino::createBlock()");
            break;
    }

    Block* block = nullptr;
    for(auto coordinate : tempCoordinate)
    {
        // mBlock
        block = new Block(mGame, 100, mGameBoard, coordinate);
        block->setTexture(texture);
        block->setScale(static_cast<float>(BLOCK_SIZE) / 60.0f);
        mBlock.push_back(block);
        // mShadowBlock
        block = new Block(mGame, 90, mGameBoard, coordinate);
        block->setTexture(texture);
        block->setScale(static_cast<float>(BLOCK_SIZE) / 60.0f);
        block->setClear(0.35f);
        mShadowBlock.push_back(block);
        mBackup.push_back(coordinate);
    }
    if(!isCoordinateCorrect(mBlock))
    {
        mGameBoard->setGameover();
    }
}