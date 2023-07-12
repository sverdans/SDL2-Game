#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <System/Window.h>
#include <Math/Vec2.h>

Window& Window::Instance()
{
	static Window instance;
	return instance;
}

bool Window::Initialize(const std::string& title, const Vec2& size, std::string& sProblem)
{
	mTitle = title;
	mSize = size;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		sProblem = std::string("Can't init SDL: ", SDL_GetError());
		return false;
	}

	mpWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		mSize.x, mSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (mpWindow == nullptr)
	{
		sProblem = std::string("Can't create window: ", SDL_GetError());
		return false;
	}

	mpRenderer = SDL_CreateRenderer(Window::Instance().GetWindowPointer(),
		-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (mpRenderer == nullptr)
	{
		sProblem = std::string("Can't create renderer: ", SDL_GetError());
		return false;
	}

	auto flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags))
	{
		sProblem = std::string("Can't init image: ", IMG_GetError());
		return false;
	}

	SetScale(mScale.x, mScale.y);
	SetClearColor(0, 0, 0);

	return true;
}

void Window::Finalize()
{
	if (mpWindow)
	{
		SDL_DestroyWindow(mpWindow);
		mpWindow = nullptr;
	}
	
	if (mpRenderer)
	{
		SDL_DestroyRenderer(mpRenderer);
		mpRenderer = nullptr;
	}

	IMG_Quit();
	SDL_Quit();
}

void Window::SetScale(int x, int y)
{
	mScale.x = x; 
	mScale.y = y;
	SDL_RenderSetScale(mpRenderer, x, y);
}
// range: 0-255
void Window::SetClearColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(mpRenderer, r, g, b, 255);
}

void Window::Draw(SDL_Texture* pTexture, SDL_Rect* srcRect, SDL_Rect* dstRect) 
{
	SDL_RenderCopy(mpRenderer, pTexture, srcRect, dstRect);
}
void Window::Clear() { SDL_RenderClear(mpRenderer); }
void Window::Render() { SDL_RenderPresent(mpRenderer); }

SDL_Window* Window::GetWindowPointer() { return mpWindow; }
SDL_Renderer* Window::GetRendererPointer() { return mpRenderer; }