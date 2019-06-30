#pragma once

#include "sprite_actor.hpp"

class Title : public SpriteActor
{
public:
    Title(class Game* game, 
          int order = 100);
};