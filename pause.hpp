#pragma once

#include "sprite_actor.hpp"

class Pause : public SpriteActor
{
public:
    Pause(class Game* game, 
          int order = 150);

    void update() override;

    void addGameBoard(class GameBoard* gameBoard);

private:
    // 現在作成されているGameBoardクラス
    std::vector<class GameBoard*> mGameBoardVector;
    // 全てのGameBoardを更新するか
    bool mIsAllUpdated;
};