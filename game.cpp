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

    // Actorなどの初期化
    initializeActor();

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
    finalizeActor();
    finalizeSDL();
}

void Game::addActor(Actor* actor)
{
    // Actor::mOrderに応じて適切な位置にActorを格納
    int order = actor->getOrder();

    auto iterator = mComponentActor.begin();
    for(;
        iterator != mComponentActor.end();
        iterator++)
    {
        if(order < (*iterator)->getOrder())
        {
            break;
        }
    }

    mComponentActor.insert(iterator, actor);
}

void Game::removeActor(Actor* actor)
{
    auto iterator = std::find(mComponentActor.begin(),
                              mComponentActor.end(),
                              actor);
    if(iterator != mComponentActor.end())
    {
        mComponentActor.erase(iterator);
    }
}

void Game::inputProcess()
{
    // ウィンドウに対するイベント
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case (SDL_QUIT):
                mIsContinuedGame = false;
                break;
        }
    }

    // キーボードの状態を取得
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // ESCAPEで終了
    if(keyboardState[SDL_SCANCODE_ESCAPE])
    {
        mIsContinuedGame = false;
    }
}

void Game::updateGame()
{

}

void Game::outputProcess()
{
    // バッファを灰色で初期化
    SDL_SetRenderDrawColor(mRenderer,
                           200,
                           200,
                           200,
                           255);
    SDL_RenderClear(mRenderer);

    // バッファの入れ替え
    SDL_RenderPresent(mRenderer);
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

    // イメージ
    IMG_Init(IMG_INIT_PNG);

    // フォントの作成
    TTF_Init();
    mFont = TTF_OpenFont("font/Cica-Bold.ttf", 24);
    if(!mFont)
    {
        SDL_Log("Failed to create font: %s", SDL_GetError());
        return false;
    }
    
    return true;
}

void Game::finalizeSDL()
{
    // フォント
    TTF_CloseFont(mFont);
    TTF_Quit();

    // イメージ
    IMG_Quit();

    // レンダラー、ウィンドウ
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);

    SDL_Quit();
}

void Game::initializeActor()
{

}

void Game::finalizeActor()
{

}