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

	Vec2 mSize { 800, 600 };
	SDL_Window* mpWindow { nullptr };
	std::string mTitle;

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

	bool Initialize(const std::string& title, const Vec2& size, std::string& problem)
	{
		mTitle = title;
		mSize = size;

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			problem = std::string("Can't init SDL: ", SDL_GetError());
			return false;
		}

		mpWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			mSize.mX, mSize.mY, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (mpWindow == nullptr)
		{
			problem = std::string("Can't create window: ", SDL_GetError());
			return false;
		}

		return true;
	}

	void Finalize()
	{
		if (!mpWindow)
			return;

		SDL_DestroyWindow(mpWindow);
		mpWindow = nullptr;

		SDL_Quit();
	}

	SDL_Window* GetWindowPointer() { return mpWindow; }
};