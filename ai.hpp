#pragma once

#include "actor.hpp"

class AI : public Actor
{
public:
    AI(class Game* game,
       int order,
       class GameBoard* gameBoard);

private:
    //  所属するGameBoardクラス
    class GameBoard* mGameBoard;
    
};