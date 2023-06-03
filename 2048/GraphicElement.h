#pragma once
#include <SDL.h>

class GraphicElement
{
protected:
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	
	SDL_Texture* background;
	SDL_Rect backgroundInFile = { 0, 0, 258, 322 };
	SDL_Rect backgroundPosition;

	void calculateWindowChanges(int width, int height)
	{
		int original_width = 258;
		int original_height = 322;

		if (width < original_width || height < original_height)
		{
			width = backgroundPosition.w = original_width;
			height = backgroundPosition.h = original_height;

			SDL_SetWindowSize(gameInitializer.window, backgroundPosition.w, backgroundPosition.h);
		}
		else if (width <= height)
		{
			backgroundPosition.h = height;
			backgroundPosition.w = original_width * (double)backgroundPosition.h / original_height;
			while (backgroundPosition.w > width)
			{
				backgroundPosition.h--;
				backgroundPosition.w = original_width * (double)backgroundPosition.h / original_height;
			}
		}
		else
		{
			backgroundPosition.w = width;
			backgroundPosition.h = original_height * (double)backgroundPosition.w / original_width;
			while (backgroundPosition.h > height)
			{
				backgroundPosition.w--;
				backgroundPosition.h = original_height * (double)backgroundPosition.w / original_width;
			}
		}

		backgroundPosition.x = (width - backgroundPosition.w) / 2;
		backgroundPosition.y = (height - backgroundPosition.h) / 2;
	}

public:
	virtual void setWindowResolution(int width, int height) = 0;
};