#include "game.hpp"

Game::Game():
    mIsContinuedGame(true)
{
    
}

bool Game::initialize()
{
    return true;
}

void Game::runLoop()
{
    while(mIsContinuedGame)
    {
        inputProcess();
        updateGame();
        outputProcess();
    }
}

void Game::shutdown()
{

}

void Game::inputProcess()
{

}

void Game::updateGame()
{

}

void Game::outputProcess()
{

}