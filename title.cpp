#include "title.hpp"
#include "controller.hpp"

Title::Title(Game* game, int order):
    SpriteActor(game, order)
{
    mPosition.set(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/title.png"));
}

void Title::update()
{
    // ボタンが押された場合、画面の移動
    auto keyboardState = mGame->getKeyboardState();
    for(auto key : keyboardState)
    {
        if(key != 0)
        {
            mGame->getController()->changeMode(Controller::PLAYING);
        }
    }
}