#pragma once

#include "sprite_actor.hpp"

class Mode : public SpriteActor
{
public:
    explicit Mode(class Game* game,
                  int order = 100);
};