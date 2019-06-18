#pragma once

#include "sprite_actor.hpp"

class GameBoard : public SpriteActor
{
public:
    GameBoard(class Game* game, int order);

    // いくつかの関数の実行
    void update(int currentFrame) override;
};