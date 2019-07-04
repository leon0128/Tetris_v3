#pragma once

#include "sprite_actor.hpp" 

class Mode : public SpriteActor
{
public:
    explicit Mode(class Game* game,
                  EMode mode,
                  int order = 100);

    // 選択されている場合の影の設定
    void draw(SDL_Renderer* renderer) override;

    // メンバ変数の設定、取得
    const bool isSelected() const {return mIsSelected;}
    void changeSelected(const bool isSelected) {mIsSelected = isSelected;}
    const EMode getMode() const {return mMode;}

private:
    // mModeに応じてテクスチャの設定
    void setTextureFromMode();

    EMode mMode;
    // 選択されているか
    bool mIsSelected;
};