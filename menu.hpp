#pragma once

#include "sprite_actor.hpp"

class Menu : public SpriteActor
{
public:
    explicit Menu(class Game* game,
                  int order = 50);
    ~Menu();

    void update() override;

private:
    // 表示されるModeを格納する配列
    std::vector<class Mode*> mModeVector;
    // 現在どのモードを選んでいるか
    EMode mSelectMode;
};