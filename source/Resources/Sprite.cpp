#include <Resources/Sprite.h>
#include <System/Window.h>

Sprite::Sprite(SDL_Texture* pTexture, const SDL_Rect& RectInFile)
	: mpTexture(pTexture), mRectInFile(RectInFile) { }

void Sprite::Render(const Vec2& position, const Vec2& size)
{
	Window::Instance().Draw();
}