#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#define gameInitializer GameInitializer::get_instance()

using namespace std;

class GameInitializer final
{
private:

	GameInitializer() = default;
	~GameInitializer() = default;
	GameInitializer(const GameInitializer&) = delete;
	GameInitializer& operator = (GameInitializer&) = delete;
	
public:

	int screen_width = 258 * 2;
	int screen_height = 322 * 2;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	static GameInitializer& get_instance()
	{
		static GameInitializer instance;
		return instance;
	}

	bool initialize()
	{
		bool ok = true;

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cout << "Can't init: " << SDL_GetError() << std::endl;
			ok = false;
		}

		window = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window == nullptr)
		{
			std::cout << "Can't create window: " << SDL_GetError() << std::endl;
			ok = false;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr)
		{
			std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
			ok = false;
		}

		SDL_SetRenderDrawColor(renderer, 117, 117, 117, 255);

		int flags = IMG_INIT_PNG;
		if (!(IMG_Init(flags) & flags))
		{
			std::cout << "Can't init image: " << IMG_GetError() << std::endl;
			ok = false;
		}

		float scale_x = 1;
		float scale_y = 1;

		SDL_RenderSetScale(renderer, scale_x, scale_y);
		return ok;
	}

	void quitGame()
	{
		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_DestroyRenderer(renderer);
		renderer = nullptr;

		SDL_Quit();
		IMG_Quit();
	}
};

enum eMenuItem
{
	inIntro = -2,
	inMenu,

	continueGame,
	newGame,
	inRecords,
	howToPlay,
	exitGame,

	inGame,
	gameOver
};

class Player final
{
public:
	char name[13];
	int score;
};