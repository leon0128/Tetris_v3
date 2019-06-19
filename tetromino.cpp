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
    for(auto block : mBlock)
    {
        block->updatePosition();
    }
}

void Tetromino::parallelMove(int displacement)
{
    if(displacement == 0)
    {
        return;
    }

    for(auto block : mBlock)
    {
        Vector2 temp = block->getCoordinate();
        temp.x += displacement;
        block->setCoordinate(temp);
    }
}

void Tetromino::verticalMove(int displacement)
{
    if(displacement == 0 &&
       mDownFrame < DROP_COUNT)
    {
        return;
    }

    if(displacement != 0)
    {
        for(auto block : mBlock)
        {
            Vector2 temp = block->getCoordinate();
            temp.y += displacement;
            block->setCoordinate(temp);
        }
    }

    if(displacement == 0 &&
       mDownFrame >= DROP_COUNT)
    {
        for(auto block : mBlock)
        {
            Vector2 temp = block->getCoordinate();
            temp.y += -1;
            block->setCoordinate(temp);
        }
    }
    mDownFrame = mGame->getFrameCount();
}

void Tetromino::rotationMove(int direction)
{

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
    }
}