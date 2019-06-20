#include "game.hpp"

#include "actor.hpp"
#include "sprite_actor.hpp"
#include "back_ground.hpp"
#include "game_board.hpp"

Game::Game():
    mIsContinuedGame(true),
    mTicksCount(0),
    mFrameCount(0),
    mCurrentKeyboardState(nullptr),
    mKeepedFrame(nullptr),
    mKeyboardSize(0),
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

    // mTextureMapからファイル名のテクスチャを探し見つけたら返す
    auto iterator = mTextureMap.find(filename);
    if(iterator != mTextureMap.end())
    {
        texture = iterator->second;
        return texture;
    }
    else
    {
        SDL_Log("Can not find texture for specified filename.");
        return texture;
    }
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

int Game::getKeepedFrame(SDL_Scancode scancode)
{
    SDL_Log("%d, %d", mKeepedFrame[scancode], scancode);
    return mKeepedFrame[scancode];
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


    // mKeyboardStateの状態の変更
    mCurrentKeyboardState = SDL_GetKeyboardState(NULL);
    for(int i = 0; i < mKeyboardSize; i++)
    {
        if(mCurrentKeyboardState[i] == 0)
        {
            mKeepedFrame[i] = 0;
        }
        else
        {
            SDL_Log("%d", i);
            mKeepedFrame += 1;
        }
    }

    // ESCAPEで終了
    if(mCurrentKeyboardState[SDL_SCANCODE_ESCAPE])
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
        actor->update();
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
    
    // テクスチャの作成
    createTexture("image/game_board.png");
    createTexture("image/side_board.png");
    createTexture("image/score_board.png");
    createTexture("image/back_ground_01.png");
    createTexture("image/back_ground_02.png");

    createTexture("image/blocks/i.png");
    createTexture("image/blocks/j.png");
    createTexture("image/blocks/l.png");
    createTexture("image/blocks/o.png");
    createTexture("image/blocks/s.png");
    createTexture("image/blocks/z.png");
    createTexture("image/blocks/t.png");

    createTexture("image/tetromino/i_mino.png");
    createTexture("image/tetromino/j_mino.png");    
    createTexture("image/tetromino/l_mino.png");
    createTexture("image/tetromino/o_mino.png");
    createTexture("image/tetromino/s_mino.png");
    createTexture("image/tetromino/t_mino.png");
    createTexture("image/tetromino/z_mino.png");

    return true;
}

void Game::finalizeSDL()
{
    // テクスチャ
    for(auto texture : mTextureMap)
    {
        SDL_DestroyTexture(texture.second);
    }
    mTextureMap.clear();

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
    // mKeyboardStateの初期化
    SDL_GetKeyboardState(&mKeyboardSize);
    mCurrentKeyboardState = new Uint8[mKeyboardSize];
    mKeepedFrame = new int[mKeyboardSize];

    // 乱数の初期化
    srand((unsigned int)time(NULL));
    // 主要オブジェクト
    new BackGround(this, 10);
    new GameBoard(this, 30);
}

void Game::finalizeActor()
{
    mComponentActor.clear();
}

void Game::createTexture(const std::string& filename)
{
    SDL_Texture* texture = nullptr;

    // テクスチャがすでに作成されている場合、Textureは作成しない
    auto iterator = mTextureMap.find(filename);
    if(iterator != mTextureMap.end())
    {
        SDL_Log("%s texture is already created.", filename.c_str());
        return;
    }
    
    // ファイルのロード
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if(!surface)
    {
        SDL_Log("Failed to load texture file: %s", SDL_GetError());
        return;
    }

    // ファイルをテクスチャに変換
    texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    if(!texture)
    {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return ;
    }

    // mTextureMapに作成したTextureの追加
    mTextureMap.emplace(filename.c_str(), texture);
}