#pragma once

#include "sprite_actor.hpp"

class Block : public SpriteActor
{
public:
    Block(class Game* game,
          int order,
          class GameBoard* gameBoald);

private:
    class GameBoard* mGameBoard;
};