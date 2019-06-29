#pragma once

#include "game.hpp"

class Controller
{
public:
    Controller(class Game* game);

private:
    void createGameBoard() const;

    // Gameオブジェクトのポインタ
    class Game* mGame;
};