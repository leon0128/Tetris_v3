#include "controller.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"

Controller::Controller(Game* game, int order):
    Actor(game, order)
{
    createGameBoard();
}

void Controller::createGameBoard() const
{
    // GameBoardを作成
    new BackGround(mGame, 10);
    new GameBoard(mGame, 30);
}