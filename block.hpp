#pragma once

#include "sprite_actor.hpp"

class Block : public SpriteActor
{
public:
    Block(class Game* game,
          int order,
          class GameBoard* gameBoald,
          class Vector2* coordinate);

private:
    class GameBoard* mGameBoard;
    class Vector2* mCoordinate;
};