#pragma once

#include "sprite_actor.hpp"

class SideBoard : public SpriteActor
{
public:
    SideBoard(class Game* game,
              int order,
              class GameBoard* gameBoard);
private:
    class GameBoard* mGameBoard;
    enum EType mType;
};