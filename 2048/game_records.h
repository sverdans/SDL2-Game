#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include "GameInitializer.h"
#include "GraphicElement.h"

using namespace std;

class game_records final : public GraphicElement
{
private:
	SDL_Texture* table_header;
	SDL_Rect header_in_file = { 0, 0, 258, 34 };
	SDL_Rect header_position;

	SDL_Texture* alphabet;
	SDL_Rect letter_in_file;
	SDL_Rect letter_position;
	vector<string> names;

	int name_y_shift;
	int number_x_shift;
	int name_x_shift;
	int y_interval;

	int current_record;

	SDL_Texture* numbers;
	SDL_Rect number_in_file;
	SDL_Rect number_position;
	vector<int> score;

	void draw_records()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

		string number;
		int string_name_x;
		int string_number_x;
		int string_y;
		int k = 0;

		int end = current_record + 9;
		for (int i = current_record; i < end; i++, k++)
		{
			if (i < names.size())
			{
				string_name_x = name_x_shift;
				string_y = name_y_shift + k * y_interval;

				for (int j = 0; j < 13; j++)
				{
					int x = names.at(i)[j];
					letter_in_file = { (x - 97) * 13, 0, 11, 14 };
					if (x == 105)
						letter_in_file.w = 5;
					if (x == 113)
						letter_in_file.w = 13;
					letter_position = { string_name_x, string_y, letter_in_file.w * backgroundPosition.w / 258, letter_in_file.h * backgroundPosition.h / 322 };
					SDL_RenderCopy(renderer, alphabet, &letter_in_file, &letter_position);
					string_name_x += letter_position.w;
				}

				string_number_x = number_x_shift;
				number = to_string(score.at(i));

				for (int j = 0; j < number.length(); j++)
				{
					int x = number[j] - '0';
					number_in_file = { x * 11, 0, 11, 14 };
					if (x == 1)
						number_in_file.w = 7;

					number_position = { string_number_x, string_y, number_in_file.w * backgroundPosition.w / 258, number_in_file.h * backgroundPosition.h / 322 };
					SDL_RenderCopy(renderer, numbers, &number_in_file, &number_position);
					string_number_x += number_position.w;
				}
			}
		}
		SDL_RenderCopy(renderer, table_header, &header_in_file, &header_position);
		SDL_RenderPresent(renderer);

	}

public:
	game_records(SDL_Renderer* renderer, int screen_width, int screen_height)
	{
		current_record = 0;
		this->renderer = renderer;

		surface = IMG_Load("textures/top_of_table.png");
		table_header = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/records.png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/numbers.png");
		numbers = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/alphabet.png");
		alphabet = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		Player player;
		ifstream fin("records.bin", ios::binary);
		if (fin.is_open())
		{
			while (fin.read((char*)&player, sizeof(Player)))
			{
				names.push_back(player.name);
				score.push_back(player.score);
			}
			fin.close();
		}
		setWindowResolution(screen_width, screen_height);
	}

	~game_records()
	{
		SDL_DestroyTexture(table_header);
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(numbers);
		SDL_DestroyTexture(alphabet);
	}

	void setWindowResolution(int width, int height) override
	{
		int original_width = 258;
		int original_height = 322;

		calculateWindowChanges(width, height);

		header_position.x = backgroundPosition.x;
		header_position.y = backgroundPosition.y;
		header_position.w = backgroundPosition.w;
		header_position.h = backgroundPosition.h * 34 / original_height;

		name_x_shift = backgroundPosition.w * 7 / original_width + backgroundPosition.x;
		number_x_shift = backgroundPosition.w * 187 / original_width + backgroundPosition.x;
		name_y_shift = backgroundPosition.h * 43 / original_height + backgroundPosition.y;
		y_interval = backgroundPosition.h * 32 / original_height;

		draw_records();
	}

	void next_record()
	{
		if (current_record + 9 < names.size())
			current_record++;
		draw_records();
	}

	void prev_record()
	{
		if (current_record > 0)
			current_record--;
		draw_records();
	}
};
