#pragma once

#include "sprite_actor.hpp" 

class Mode : public SpriteActor
{
public:
    explicit Mode(class Game* game,
                  EMode mode,
                  int order = 100);
    
private:
    // mModeに応じてテクスチャの設定
    void setTextureFromMode();

    EMode mMode;
};