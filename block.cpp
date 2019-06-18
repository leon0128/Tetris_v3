#include "block.hpp"

Block::Block(Game* game, 
             int order, 
             GameBoard* gameBoard,
             Vector2* coordinate):
    SpriteActor(game, order),
    mGameBoard(gameBoard)
{
    // mCoordinateの値を初期化
    mCoordinate = new Vector2(coordinate->x, coordinate->y);
    // 大きさを合わせる
    setScale(BLOCK_SIZE / 60.0f);
}