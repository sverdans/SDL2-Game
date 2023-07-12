#include <Resources/Sprite.h>
#include <System/Window.h>

Sprite::Sprite(SDL_Texture* pTexture, const SDL_Rect& RectInFile)
	: mpTexture(pTexture), mRectInFile(RectInFile) { }

void Sprite::Render(const Vec2& position, const Vec2& size)
{
	SDL_Rect dstRec { position.x, position.y, size.x, size.y };
	Window::Instance().Draw(mpTexture, &mRectInFile, &dstRec);
}