#pragma once

#include "ai.hpp"

class LeonAI : public AI
{
public:
    LeonAI(class Game* game,
           int order,
           class GameBoard* gameBoard);
    
    // 各々実装
    void calculate() override;
};