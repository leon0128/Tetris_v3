#include "block.hpp"

Block::Block(Game* game, int order, GameBoard* gameBoard):
    SpriteActor(game, order),
    mGameBoard(gameBoard)
{
}