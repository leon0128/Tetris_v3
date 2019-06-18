#include "game.hpp"

Game::Game():
    mIsContinuedGame(true),
    mTicksCount(0),
    mFrameCount(0),    
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

SDL_Texture* Game::getTexture(const std::string& filename)
{
    SDL_Texture* texture = nullptr;
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

void Game::addSpriteActor(SpriteActor* actor)
{
    // Actor::mOrderに応じて適切な位置にActorを格納
    int order = actor->getOrder();

    auto iterator = mComponentSpriteActor.begin();
    for(;
        iterator != mComponentSpriteActor.end();
        iterator++)
    {
        if(order < (*iterator)->getOrder())
        {
            break;
        }
    }

    mComponentSpriteActor.insert(iterator, actor);
}

void Game::removeSpriteActor(SpriteActor* actor)
{
    auto iterator = std::find(mComponentSpriteActor.begin(),
                              mComponentSpriteActor.end(),
                              actor);
    if(iterator != mComponentSpriteActor.end())
    {
        mComponentSpriteActor.erase(iterator);
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
    // 60fps
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)){}
    // mTicksCountとmFrameCountを更新
    mTicksCount = SDL_GetTicks();
    mFrameCount ++;

    // アクターの更新
    for(auto actor : mComponentActor)
    {
        actor->update(mFrameCount);
    }
    // いらないActorの削除
    mRemoveActor.clear();
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

    for(auto actor : mComponentSpriteActor)
    {
        actor->draw(mRenderer);
    }

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
    mComponentActor.clear();
}