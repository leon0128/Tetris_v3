#include "side_board.hpp"

SideBoard::SideBoard(Game* game, int order, GameBoard* gameBoard):
    SpriteActor(game, order),
    mGameBoard(gameBoard),
    mType(NONE)
{
    setTexture(mGame->getTexture("image/side_board.png"));
    setClear(0.65f);
}
    