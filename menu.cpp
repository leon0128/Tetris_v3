#include "menu.hpp"

#include "mode.hpp"
#include "controller.hpp"

Menu::Menu(Game* game, int order):
    SpriteActor(game, order),
    mSelectMode(MARATHON)
{
    mPosition.set(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    setTexture(mGame->getTexture("image/back_ground_01.png"));

    mModeVector.emplace_back(new Mode(mGame, MARATHON));
    mModeVector.at(0)->changeSelected(true);
}

Menu::~Menu()
{
    for(int i = 0; i < (int)mModeVector.size(); i++)
    {
        delete mModeVector.at(i);
    }
}

void Menu::update()
{
    if(mGame->getKeyboardState().at(SDL_SCANCODE_J) == 1 ||
       mGame->getKeyboardState().at(SDL_SCANCODE_SPACE) == 1)
    {
        mGame->getController()->changeMode(Controller::PLAYING);
    }
}