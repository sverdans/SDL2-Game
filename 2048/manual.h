#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "GameInitializer.h"
#include "GraphicElement.h"

class manual final : public GraphicElement
{
private:
	SDL_Texture* alphabet;
	SDL_Texture* numbers;
	SDL_Texture* signs;

	SDL_Rect symbol_in_file;
	SDL_Rect symbol_position;

	int strings_counter;
	int string_start_x;
	int string_start_y;
	int y_interval;
	int max_length;
	int symbol_height;

	string instruction[47];
	int cur_string;

	void draw_manual()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

		int k = 0;
		for (int i = cur_string; i < cur_string + 15; i++, k++)
		{
			int string_y = k * y_interval + string_start_y;
			int string_x = string_start_x;

			int string_length = 0;
			int gaps_counter = 0;
			int gap_length = 0;

			for (int j = 0; j < instruction[i].length(); j++)
			{
				int x = instruction[i][j];
				if (x == 32)
					gaps_counter++;
				else if (x >= 97 && x <= 122)
				{
					if (x == 105)
						string_length += 5 * backgroundPosition.w / 258;
					else if (x == 113)
						string_length += 13 * backgroundPosition.w / 258;
					else
						string_length += 11 * backgroundPosition.w / 258;
				}
				else if (x >= 48 && x <= 57)
				{
					if (x == 49)
						string_length += 7 * backgroundPosition.w / 258;
					else
						string_length += 11 * backgroundPosition.w / 258;
				}
				else if (x == 46 || x == 44)
					string_length += 4 * backgroundPosition.w / 258;
				else if (x == 39)
					string_length += 7 * backgroundPosition.w / 258;
			}

			if (gaps_counter > 0)
				gap_length = (max_length - string_length) / gaps_counter;

			for (int j = 0; j < instruction[i].length(); j++)
			{
				int x = instruction[i][j];
				if (x == 32)
				{
					symbol_in_file = { 400, 0, 4, 14 };
					symbol_position = { string_x, string_y, gap_length, symbol_height };
					SDL_RenderCopy(renderer, alphabet, &symbol_in_file, &symbol_position);
					string_x += symbol_position.w;
				}
				else if (x >= 97 && x <= 122)
				{
					symbol_in_file = { (x - 97) * 13, 0, 11, 14 };
					if (x == 105)
						symbol_in_file.w = 5;
					if (x == 113)
						symbol_in_file.w = 13;
					symbol_position = { string_x, string_y, symbol_in_file.w * backgroundPosition.w / 258, symbol_height };
					SDL_RenderCopy(renderer, alphabet, &symbol_in_file, &symbol_position);
					string_x += symbol_position.w;
				}
				else if (x >= 48 && x <= 57)
				{
					symbol_in_file = { (x - 48) * 11, 0, 11, 14 };
					if (x == 49)
						symbol_in_file.w = 7;

					symbol_position = { string_x, string_y, symbol_in_file.w * backgroundPosition.w / 258, symbol_height };
					SDL_RenderCopy(renderer, numbers, &symbol_in_file, &symbol_position);
					string_x += symbol_position.w;
				}
				else if (x == 46)
				{
					symbol_in_file = { 0, 0, 4, 14 };
					symbol_position = { string_x, string_y, symbol_in_file.w * backgroundPosition.w / 258, symbol_height };
					SDL_RenderCopy(renderer, signs, &symbol_in_file, &symbol_position);
					string_x += symbol_position.w;
				}
				else if (x == 44)
				{
					symbol_in_file = { 12, 0, 4, 14 };
					symbol_position = { string_x, string_y, symbol_in_file.w * backgroundPosition.w / 258, symbol_height };
					SDL_RenderCopy(renderer, signs, &symbol_in_file, &symbol_position);
					string_x += symbol_position.w;
				}
				else if (x == 39)
				{
					symbol_in_file = { 24, 0, 7, 14 };
					symbol_position = { string_x, string_y, symbol_in_file.w * backgroundPosition.w / 258, symbol_height };
					SDL_RenderCopy(renderer, signs, &symbol_in_file, &symbol_position);
					string_x += symbol_position.w;
				}
			}
		}

		SDL_RenderPresent(renderer);
	}

public:

	manual(SDL_Renderer* renderer, int screen_width, int screen_height)
	{
		cur_string = 0;
		this->renderer = renderer;

		instruction[0] = "1. each round, a tile of";
		instruction[1] = "denominations '2' or '4'";
		instruction[2] = "appears.";

		instruction[4] = "2. by pressing the arrow,";
		instruction[5] = "the player can throw";
		instruction[6] = "off all the tiles of the";
		instruction[7] = "playing field in one of";
		instruction[8] = "4 directions. if, when";
		instruction[9] = "dropped, two tiles of";
		instruction[10] = "the same denomination";
		instruction[11] = "'bump' into one another,";
		instruction[12] = "then they turn into one,";
		instruction[13] = "the denomination of";
		instruction[14] = "which is equal to the";
		instruction[15] = "sum of the connected";
		instruction[16] = "tiles. after each move,";
		instruction[17] = "a new tile of '2' or '4'";
		instruction[18] = "appears on the free";
		instruction[19] = "section of the field.";
		instruction[20] = "if the location of";
		instruction[21] = "the tiles or their";
		instruction[22] = "denomination does not";
		instruction[23] = "change when the button";
		instruction[24] = "is pressed, the move";
		instruction[25] = "is not made.      ";

		instruction[27] = "3. if there are more";
		instruction[28] = "than two tiles of the";
		instruction[29] = "same denomination in one";
		instruction[30] = "line or in one column,";
		instruction[31] = "then when dropped, they";
		instruction[32] = "begin to connect from";
		instruction[33] = "the side to which they";
		instruction[34] = "were directed.      ";

		instruction[36] = "4. for each connection,";
		instruction[37] = "the game points are";
		instruction[38] = "increased by the face";
		instruction[39] = "value of the resulting";
		instruction[40] = "tile.";

		instruction[42] = "5. the game ends in";
		instruction[43] = "defeat if after the next";
		instruction[44] = "move it is impossible to";
		instruction[45] = "perform an action.      ";

		strings_counter = 46;

		surface = IMG_Load("textures/manual.png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/numbers.png");
		numbers = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/alphabet.png");
		alphabet = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/signs.png");
		signs = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		setWindowResolution(screen_width, screen_height);
	}

	~manual()
	{
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(numbers);
		SDL_DestroyTexture(alphabet);
		SDL_DestroyTexture(signs);
	}

	void setWindowResolution(int width, int height) override
	{
		int original_width = 258;
		int original_height = 322;

		calculateWindowChanges(width, height);

		string_start_x = backgroundPosition.x + (8 * backgroundPosition.w / original_width);
		string_start_y = backgroundPosition.y + (14 * backgroundPosition.w / original_width);
		y_interval = 20 * backgroundPosition.h / original_height;
		max_length = 244 * backgroundPosition.h / original_height;
		symbol_height = 14 * backgroundPosition.h / 322;
		draw_manual();
	}

	void up()
	{
		if (cur_string > 0)
		{
			cur_string--;
			draw_manual();
		}
	}

	void down()
	{
		if (cur_string + 15 < strings_counter)
		{
			cur_string++;
			draw_manual();
		}
	}
};