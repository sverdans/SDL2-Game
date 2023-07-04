#pragma once
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <Math/Vec2.h>

class Window final
{
private:
	Window() = default;
	~Window() = default;

	std::string mTitle;

	Vec2 mSize { 800, 600 };
	Vec2 mScale { 1, 1 };

	SDL_Window* mpWindow { nullptr };
    SDL_Renderer* mpRenderer { nullptr };

public:

	Window(const Window&) = delete;
	Window(Window&&) = delete;

	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	static Window& Instance()
	{
		static Window instance;
		return instance;
	}

	bool Initialize(const std::string& title, const Vec2& size, std::string& sProblem)
	{
		mTitle = title;
		mSize = size;

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			sProblem = std::string("Can't init SDL: ", SDL_GetError());
			return false;
		}

		mpWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			mSize.mX, mSize.mY, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

		SDL_RenderSetScale(mpRenderer, mScale.mX, mScale.mY);
		SetClearColor(0, 0, 0);

		return true;
	}

	void Finalize()
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

	void SetClearColor(int r, int g, int b) { SDL_SetRenderDrawColor(mpRenderer, r, g, b, 255); }

	void Draw() { }
	void Clear() { SDL_RenderClear(mpRenderer); }
    void Render() { SDL_RenderPresent(mpRenderer); }

	SDL_Window* GetWindowPointer() { return mpWindow; }
	SDL_Renderer* GetRendererPointer() { return mpRenderer; }

};