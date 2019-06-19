#include "block.hpp"
#include "game_board.hpp"

Block::Block(Game* game, 
             int order, 
             GameBoard* gameBoard,
             Vector2* coordinate):
    SpriteActor(game, order),
    mGameBoard(gameBoard)
{
    // mCoordinateの値を初期化
    mCoordinate = new Vector2(coordinate->x, coordinate->y);
}

void Block::updatePosition()
{
    Vector2* center = mGameBoard->getPosition();
    mPosition->setX(center->x + 
                    (mCoordinate->x - 4) * BLOCK_SIZE - 
                    BLOCK_SIZE / 2);
    mPosition->setY(center->y -
                    (mCoordinate->y - 10) * BLOCK_SIZE - 
                    BLOCK_SIZE / 2);
    updateRectangle();
}