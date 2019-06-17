#include "game.hpp"

Game::Game():
    mIsContinuedGame(true),
    mWindow(nullptr),
    mRenderer(nullptr),
    mFont(nullptr)
{
    
}

bool Game::initialize()
{
    // sdl関連の初期化
    if(!initializeSDL())
    {
        return false;
    }
    
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

void Game::finalize()
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

bool Game::initializeSDL()
{
    // VIDEOとAUDIOの初期化
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Could not initialize SDL: %s", SDL_GetError());
        return false;
    }

    // ウィンドウ作成
    mWindow = SDL_CreateWindow("TETRIS",
                                100,
                                100,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT,
                                0);
    if(!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // レンダラーの作成
    mRenderer = SDL_CreateRenderer(mWindow,
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    if(!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // フォントの作成
    TTF_Init();
    mFont = TTF_OpenFont("font/Cica_Bold.ttf", 24);
    if(!mFont)
    {
        SDL_Log("Failed to create font: %s", SDL_GetError());
        return false;
    }
    
    return true;
}