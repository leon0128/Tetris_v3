#pragma once

#include "sprite_actor.hpp"

class ScoreBoard : public SpriteActor
{
public:
    ScoreBoard(class Game* game,
               int order,
               class GameBoard* gameBoard);
private:
    // Description用のテクスチャを作成し、


    std::vector<SDL_Texture*> mDescriptionTexture;
    class GameBoard* mGameBoard;
};