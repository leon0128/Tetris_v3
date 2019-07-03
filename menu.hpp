#pragma once

#include "sprite_actor.hpp"

class Menu : public SpriteActor
{
public:
    explicit Menu(class Game* game,
                  int order = 50);
    
};