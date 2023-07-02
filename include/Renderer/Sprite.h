#pragma once
#include <memory>

#include <SDL.h>
#include <SDL_image.h>

#include <Math/Vec2.h>
#include <Renderer/Renderer.h>

class Sprite
{
private:
    std::shared_ptr<SDL_Texture> mpTexture;
    SDL_Rect mRectInFile;

public:
    Sprite(std::shared_ptr<SDL_Texture> pTexture, const SDL_Rect& RectInFile) 
        : mpTexture(pTexture), mRectInFile(RectInFile) { }

    void Render(const Vec2& position, const Vec2& size)
    {
        Renderer::Instance().Render();
    }
};
