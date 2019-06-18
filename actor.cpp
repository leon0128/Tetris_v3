#include "actor.hpp"

Actor::Actor(Game* game, int order):
    mGame(game),
    mOrder(order),
    mTexture(nullptr)
{
    mGame->addActor(this);
}

Actor::~Actor()
{
    mGame->removeActor(this);
}