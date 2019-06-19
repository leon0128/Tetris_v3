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
    mDownFrame(0)
{
    createBlock(mType);
}

void Tetromino::update()
{
    mBackup.clear();
    for(auto block : mBlock)
    {
        mBackup.push_back(block->getCoordinate());
        block->updatePosition();
    }
}

void Tetromino::parallelMove(int direction)
{
    if(direction == 0)
    {
        return;
    }

    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.x += direction;
        block->setCoordinate(temp);
    }
}

void Tetromino::verticalMove(int direction)
{
    if(direction == 0 &&
       mGame->getFrameCount() - mDownFrame  < DROP_COUNT)
    {
        return;
    }

    // 1マス移動
    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.y -= 1;
        block->setCoordinate(temp);
    }

    // 下に下がった時のフレームの更新
    mDownFrame = mGame->getFrameCount();
}

void Tetromino::rotationMove(int direction)
{
    if(direction == 0)
    {
        return;
    }

    // 優先度の高いブロックを中心として処理
    for(int i = 0; i < (int)mBlock.size(); i++)
    {
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
        break;
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