#pragma once
#include <memory>
#include <SDL.h>

#include <Math/Vec2.h>

class Sprite
{
private:
    SDL_Texture* mpTexture;
    SDL_Rect mRectInFile;

public:
    Sprite(SDL_Texture* pTexture, const SDL_Rect& RectInFile);

    void Render(const Vec2& position, const Vec2& size);
};
