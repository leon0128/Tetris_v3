#pragma once

#include "actor.hpp"

class Controller : public Actor
{
public:
    Controller(class Game* game,
               int order);
};