#pragma once
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <System/Window.h>
#include <Math/Vec2.h>
#include <Math/Vec3.h>

class Renderer final
{
private:
	Renderer() = default;
	~Renderer() = default;

	Vec2 mScale { 1, 1 };
    SDL_Renderer* mpRenderer;

public:
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;

	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	static Renderer& Instance()
	{
		static Renderer instance;
		return instance;
	}

	bool Initialize(std::string problem)
	{
		mpRenderer = SDL_CreateRenderer(Window::Instance().GetWindowPointer(),
			-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (mpRenderer == nullptr)
		{
			problem = std::string("Can't create renderer: ", SDL_GetError());
			return false;
		}

		auto flags = IMG_INIT_PNG;
		if (!(IMG_Init(flags) & flags))
		{
			problem = std::string("Can't init image: ", IMG_GetError());
			return false;
		}

		SDL_RenderSetScale(mpRenderer, mScale.mX, mScale.mY);
		SetClearColor(0, 0, 0);

		return true;
	}

	void Finalize()
	{
		if (!mpRenderer)
			return;

		SDL_DestroyRenderer(mpRenderer);
		mpRenderer = nullptr;

		IMG_Quit();
	}

	void SetClearColor(int r, int g, int b)
	{
		SDL_SetRenderDrawColor(mpRenderer, r, g, b, 255);
	}

    void Clear()
    {
		SDL_RenderClear(mpRenderer);
    }

    void Render()
    {

    }
    
    void Draw()
    {
		SDL_RenderPresent(mpRenderer);
    }
};