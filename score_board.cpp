#include "score_board.hpp"

ScoreBoard::ScoreBoard(Game* game, int order, GameBoard* gameBaord):
    SpriteActor(game, order),
    mGameBoard(gameBaord)
{
    setTexture(mGame->getTexture("image/score_board.png"));
    setClear(0.65f);
}