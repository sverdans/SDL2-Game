#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "GraphicElement.h"

class Menu final : public GraphicElement
{
private:
	eMenuItem position;
	
	SDL_Texture* menuItems;

	SDL_Texture* selectedItem;
	SDL_Rect selectedItemInFile = { 0, 0 , 258, 64 };
	SDL_Rect selectedItemPosition;
	int itemShift;

	void drawMenu()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
		selectedItemPosition.y = backgroundPosition.y + position * itemShift;
		SDL_RenderCopy(renderer, selectedItem, &selectedItemInFile, &selectedItemPosition);
		SDL_RenderCopy(renderer, menuItems, &backgroundInFile, &backgroundPosition);
		SDL_RenderPresent(renderer);
	}

public:
	Menu(SDL_Renderer* renderer, int screen_width, int screen_height)
	{
		this->renderer = renderer;

		position = continueGame;

		surface = IMG_Load("textures/game_menu.png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/menu_items.png");
		menuItems = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/selected_item.png");
		selectedItem = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		setWindowResolution(screen_width, screen_height);
	}

	~Menu()
	{
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(menuItems);
		SDL_DestroyTexture(selectedItem);
	}

	void setWindowResolution(int width, int height) override
	{
		int original_width = 258;
		int original_height = 322;

		calculateWindowChanges(width, height);

		itemShift = (backgroundPosition.h - (2.0 * backgroundPosition.h / original_height)) / 5.0;

		selectedItemPosition.x = backgroundPosition.x;
		selectedItemPosition.y = backgroundPosition.y;
		selectedItemPosition.w = backgroundPosition.w;
		selectedItemPosition.h = backgroundPosition.h * 64 / original_height;

		drawMenu();
	}

	void up(bool is_mouse)
	{
		int shift;
		eMenuItem new_position = eMenuItem(position - 1);
		if (new_position < continueGame)
			new_position = continueGame;
		else
		{
			int end = selectedItemPosition.y - itemShift;
			if (is_mouse == false)
				shift = itemShift / 16;
			else
				shift = itemShift / 8;

			for (selectedItemPosition.y; selectedItemPosition.y > end; selectedItemPosition.y -= shift)
			{
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
				SDL_RenderCopy(renderer, selectedItem, &selectedItemInFile, &selectedItemPosition);
				SDL_RenderCopy(renderer, menuItems, &backgroundInFile, &backgroundPosition);
				SDL_RenderPresent(renderer);
			}

			position = eMenuItem(new_position);
			drawMenu();
		}
	}

	void down(bool is_mouse)
	{
		int shift;
		int new_position = eMenuItem(position + 1);
		if (new_position > exitGame)
			new_position = exitGame;
		else
		{
			int end = selectedItemPosition.y + itemShift;
			if (is_mouse == false)
				shift = itemShift / 16;
			else
				shift = itemShift / 8;

			for (selectedItemPosition.y; selectedItemPosition.y < end; selectedItemPosition.y += shift)
			{
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
				SDL_RenderCopy(renderer, selectedItem, &selectedItemInFile, &selectedItemPosition);
				SDL_RenderCopy(renderer, menuItems, &backgroundInFile, &backgroundPosition);
				SDL_RenderPresent(renderer);
			}
			position = eMenuItem(new_position);
			drawMenu();
		}
	}

	void mouseInMenu(int x, int y)
	{
		eMenuItem new_item;
		if (x > backgroundPosition.x && x < backgroundPosition.x + backgroundPosition.w)
		{
			new_item = eMenuItem((y - backgroundPosition.y) / selectedItemPosition.h);
			if (new_item < continueGame)
				return;
			else if (new_item > exitGame)
				return;
			else if (new_item == position)
				return;
			else if (new_item > position)
				down(true);
			else if (new_item < position)
				up(true);
		}
	}

	eMenuItem enter(bool is_mouse, int x, int y)
	{
		if (is_mouse == true)
		{
			if (x > backgroundPosition.x && x < backgroundPosition.x + backgroundPosition.w)
			{
				eMenuItem new_item;
				new_item = eMenuItem((y - backgroundPosition.y) / selectedItemPosition.h);
				if (new_item == position)
					return position;
				else
					return inMenu;
			}
			return inMenu;
		}
		else
			return position;
	}
};

class GameOverMenu final : public GraphicElement
{
private:
	eMenuItem position;
	SDL_Texture* menuItems;

	SDL_Texture* selectedItem;
	SDL_Rect selectedItemInFile = { 0, 0, 128, 62 };
	SDL_Rect selectedItemPosition;
	int xLeft;
	int xRight;

	void drawMenu()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

		if (position == newGame)
			selectedItemPosition.x = xLeft;
		else
			selectedItemPosition.x = xRight;

		SDL_RenderCopy(renderer, selectedItem, &selectedItemInFile, &selectedItemPosition);
		SDL_RenderCopy(renderer, menuItems, &backgroundInFile, &backgroundPosition);
		SDL_RenderPresent(renderer);
	}

public:
	GameOverMenu(SDL_Renderer* renderer, int screen_width, int screen_height)
	{
		this->renderer = renderer;

		position = newGame;

		surface = IMG_Load("textures/game_over_menu.png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/yes_no.png");
		menuItems = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/yes_no_item.png");
		selectedItem = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		setWindowResolution(screen_width, screen_height);
	}

	~GameOverMenu()
	{
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(menuItems);
		SDL_DestroyTexture(selectedItem);
	}
	
	void setWindowResolution(int width, int height) override
	{
		int original_width = 258;
		int original_height = 322;

		calculateWindowChanges(width, height);

		selectedItemPosition.y = backgroundPosition.h * 130 / original_height + backgroundPosition.y;
		selectedItemPosition.w = backgroundPosition.w * 128 / original_width;
		selectedItemPosition.h = backgroundPosition.h * 62 / original_height;
		xLeft = backgroundPosition.x;
		xRight = backgroundPosition.x + backgroundPosition.w * 128 / original_width;
		
		drawMenu();

	}
	
	void mouseInMenu(int x, int y)
	{
		if (y > backgroundPosition.y + 0.4 * backgroundPosition.h && y < backgroundPosition.y + 0.6 * backgroundPosition.h)
		{
			if (x < backgroundPosition.x + backgroundPosition.w / 2 && x > backgroundPosition.x && position == inMenu)
				left();
			else if (x > backgroundPosition.x + backgroundPosition.w / 2 && x < backgroundPosition.x + backgroundPosition.w && position == newGame)
				right();
		}
	}

	void left()
	{
		if (position == inMenu)
		{
			int shift = selectedItemPosition.w / 16;
			position = newGame;
			for (selectedItemPosition.x; selectedItemPosition.x > xLeft; selectedItemPosition.x -= shift)
			{
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
				SDL_RenderCopy(renderer, selectedItem, &selectedItemInFile, &selectedItemPosition);
				SDL_RenderCopy(renderer, menuItems, &backgroundInFile, &backgroundPosition);
				SDL_RenderPresent(renderer);
			}
		}
		drawMenu();
	}

	void right()
	{
		if (position == newGame)
		{
			int shift = selectedItemPosition.w / 16;
			position = inMenu;
			for (selectedItemPosition.x; selectedItemPosition.x < xRight; selectedItemPosition.x += shift)
			{
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
				SDL_RenderCopy(renderer, selectedItem, &selectedItemInFile, &selectedItemPosition);
				SDL_RenderCopy(renderer, menuItems, &backgroundInFile, &backgroundPosition);
				SDL_RenderPresent(renderer);
			}
		}
		drawMenu();
	}

	eMenuItem enter(bool is_mouse, int x, int y)
	{
		if (is_mouse == true)
		{
			if (y > backgroundPosition.y + 0.4 * backgroundPosition.h && y < backgroundPosition.y + 0.6 * backgroundPosition.h)
			{
				if (x < backgroundPosition.x + backgroundPosition.w / 2 && x > backgroundPosition.x && position == newGame)
					return newGame;
				else if (x > backgroundPosition.x + backgroundPosition.w / 2 && x < backgroundPosition.x + backgroundPosition.w && position == inMenu)
					return inMenu;
				else
					return gameOver;
			}
			return gameOver;
		}
		else
			return position;
	}
};

class Intro final : public GraphicElement
{
private:
	void drawIntro()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
		SDL_RenderPresent(renderer);
	}

public:
	Intro(SDL_Renderer* renderer, int width, int height)
	{
		this->renderer = renderer;

		surface = IMG_Load("textures/petrov.a.e..png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		setWindowResolution(width, height);
	}

	~Intro()
	{
		SDL_DestroyTexture(background);
	}

	void setWindowResolution(int width, int height) override
	{
		calculateWindowChanges(width, height);
		drawIntro();
	}
};