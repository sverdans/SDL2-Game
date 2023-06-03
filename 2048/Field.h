#pragma once
#include <fstream>
#include <chrono>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "GameInitializer.h"
#include "GraphicElement.h"

using namespace std;

class Field final : public GraphicElement
{
private:
	class Cell
	{
	public:

		int x;
		int prev;
		int shift;
		bool isNew;

		Cell()
		{
			x = 0;
			prev = 0;
			shift = 0;
			isNew = false;
		}

		int operator = (const int& x)
		{
			isNew = false;
			this->x = x;
			return x;
		}

		Cell operator = (const Cell& other)
		{
			this->isNew = other.isNew;
			this->x = other.x;
			return *this;
		}

		operator int() { return x; }
	};

	enum eOffsetIndex
	{
		toLeft = -1,
		toRight = 1,
		toUp = -1,
		toDown = 1
	};

	Cell array[4][4];
	int cellCount;
	Player player;
	int bestScore;

	int fieldWidth;
	int fieldHeight;
	int widthShift;
	int heightShift;

	SDL_Texture* cellsTexture;
	SDL_Rect cellInFile = { 0, 0, 64, 64 };
	SDL_Rect cellPosition = { widthShift, heightShift, cellResolution, cellResolution };
	const int resolutionInFile = 64;
	int cellResolution;
	int cellWidthShift;
	int cellHeightShift;

	SDL_Texture* numbersTexture;
	int numberWidthShift;
	int numberHeigthShift;
	int numberHeight;
	int numberWidth;
	int numberOneWidth;

	int powerOfTwo(int number)
	{
		int power = -1;
		for (number; number > 0; number /= 2, power++);
		return power - 1;
	}

	bool addCell()
	{
		srand(chrono::system_clock::now().time_since_epoch().count());

		bool addition = false;
		int newI, newJ;

		while (addition == false)
		{
			newI = rand() % 4;
			newJ = rand() % 4;

			if (array[newI][newJ] == 0)
			{
				addition = true;
				if (rand() % 100 + 1 < 90)
					array[newI][newJ] = 2;
				else
					array[newI][newJ] = 4;
				cellCount++;
			}
		}

		drawAddedCell(newI, newJ);

		if (cellCount == 16)
		{
			bool game_over = true;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (array[i][j] == array[i + 1][j])
					{
						game_over = false;
						break;
					}
				}
				if (game_over == false)
					break;
			}

			if (game_over == true)
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (array[i][j] == array[i][j + 1])
						{
							game_over = false;
							break;
						}
					}
					if (game_over == false)
						break;
				}
			}

			return game_over;
		}

		return false;
	}

	void resetAdditionalArrays()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				array[i][j].isNew = false;
				array[i][j].shift = 0;
				array[i][j].prev = array[i][j];
			}
		}
	}

	void renderCells()
	{
		cellPosition.w = cellPosition.h = cellResolution;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cellPosition.y = i * cellResolution + cellHeightShift;
				cellPosition.x = j * cellResolution + cellWidthShift;
				cellInFile.x = resolutionInFile * powerOfTwo(array[i][j]);
				SDL_RenderCopy(renderer, cellsTexture, &cellInFile, &cellPosition);
			}
		}
	}

	void renderScore(int score, bool isBest)
	{
		SDL_Rect numberInFile;
		SDL_Rect numberPosition;

		int length = 0;
		int numbersShift;
		string numbers = to_string(score);

		for (int i = 0; i < numbers.length(); i++)
		{
			if (numbers[i] == '1')
				length += numberOneWidth;
			else
				length += numberWidth;
		}

		length -= 2 * fieldWidth / 258;
		numberWidthShift = widthShift + fieldWidth / 4 - length / 2;

		if (isBest == false)
			numbersShift = numberWidthShift;
		else
			numbersShift = numberWidthShift + 2 * cellResolution;

		int x;
		for (int i = 0; i < numbers.length(); i++)
		{
			x = numbers[i] - '0';
			numberInFile = { x * 11, 0, 11, 14 };

			if (x == 1)
			{
				numberInFile.w = 7;
				numberPosition = { numbersShift, numberHeigthShift, numberOneWidth, numberHeight };
				SDL_RenderCopy(renderer, numbersTexture, &numberInFile, &numberPosition);
				numbersShift += numberOneWidth;
			}
			else
			{
				numberPosition = { numbersShift, numberHeigthShift, numberWidth, numberHeight };
				SDL_RenderCopy(renderer, numbersTexture, &numberInFile, &numberPosition);
				numbersShift += numberWidth;
			}
		}
	}

	void drawField()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
		renderCells();
		renderScore(player.score, false);
		renderScore(bestScore, true);
		SDL_RenderPresent(renderer);
	}

	void drawAddedCell(int newI, int newJ)
	{
		for (int addedResolution = cellResolution / 2; addedResolution < cellResolution; addedResolution += cellResolution / 8)
		{
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (i == newI && j == newJ)
					{
						cellPosition.y = i * cellResolution + cellHeightShift + (cellResolution - addedResolution) / 2;
						cellPosition.x = j * cellResolution + cellWidthShift + (cellResolution - addedResolution) / 2;
						cellPosition.w = cellPosition.h = addedResolution;
						cellInFile.x = resolutionInFile * powerOfTwo(array[i][j]);
						SDL_RenderCopy(renderer, cellsTexture, &cellInFile, &cellPosition);
					}
					else
					{
						cellPosition.y = i * cellResolution + cellHeightShift;
						cellPosition.x = j * cellResolution + cellWidthShift;
						cellPosition.w = cellPosition.h = cellResolution;
						cellInFile.x = resolutionInFile * powerOfTwo(array[i][j]);
						SDL_RenderCopy(renderer, cellsTexture, &cellInFile, &cellPosition);
					}
				}
			}
			renderScore(player.score, false);
			renderScore(bestScore, true);
			SDL_RenderPresent(renderer);
		}
		drawField();
	}

	void drawNewCell()
	{
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

		int increase = fieldWidth / 258;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (array[i][j].isNew == true)
				{
					cellPosition.y = i * cellResolution - increase + cellHeightShift;
					cellPosition.x = j * cellResolution - increase + cellWidthShift;
					cellPosition.w = cellPosition.h = cellResolution + 2 * increase;
				}
				else
				{
					cellPosition.y = i * cellResolution + cellHeightShift;
					cellPosition.x = j * cellResolution + cellWidthShift;
					cellPosition.w = cellPosition.h = cellResolution;
				}

				cellInFile.x = resolutionInFile * powerOfTwo(array[i][j]);

				SDL_RenderCopy(renderer, cellsTexture, &cellInFile, &cellPosition);
			}
		}
		renderScore(player.score, false);
		renderScore(bestScore, true);
		SDL_RenderPresent(renderer);
		SDL_Delay(125);
	}

	void drawHorizontalOffset(eOffsetIndex index)
	{
		cellPosition.w = cellPosition.h = cellResolution;
		int shift = cellResolution / 10;

		for (int horizonOffset = 0; horizonOffset < cellResolution; horizonOffset += shift)
		{
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cellPosition.y = i * cellResolution + cellHeightShift;
					cellPosition.x = j * cellResolution + index * horizonOffset * array[i][j].shift + cellWidthShift;
					cellInFile.x = resolutionInFile * powerOfTwo(array[i][j].prev);
					SDL_RenderCopy(renderer, cellsTexture, &cellInFile, &cellPosition);
				}
			}

			renderScore(player.score, false);
			renderScore(bestScore, true);
			SDL_RenderPresent(renderer);
		}
		drawNewCell();
	}

	void drawVerticalOffset(eOffsetIndex index)
	{
		cellPosition.w = cellPosition.h = cellResolution;
		int shift = cellResolution / 10;

		for (int verticalOffset = 0; verticalOffset < cellResolution; verticalOffset += shift)
		{
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cellPosition.y = i * cellResolution + index * verticalOffset * array[i][j].shift + cellHeightShift;
					cellPosition.x = j * cellResolution + cellWidthShift;
					cellInFile.x = resolutionInFile * powerOfTwo(array[i][j].prev);
					SDL_RenderCopy(renderer, cellsTexture, &cellInFile, &cellPosition);
				}
			}

			renderScore(player.score, false);
			renderScore(bestScore, true);
			SDL_RenderPresent(renderer);
		}
		drawNewCell();
	}

	void writeInRecords(Player gamer)
	{
		Player temp;
		bool is_writen = false;

		ofstream fout("new_records.bin", ios::binary);
		ifstream fin("records.bin");

		if (fin.peek() != EOF)
		{
			while (fin.read((char*)&temp, sizeof(Player)))
			{
				if (gamer.score > temp.score && is_writen == false)
				{
					fout.write((char*)&gamer, sizeof(Player));
					is_writen = true;
				}
				fout.write((char*)&temp, sizeof(Player));
			}
			if (is_writen == false)
				fout.write((char*)&gamer, sizeof(Player));
		}
		else
			fout.write((char*)&gamer, sizeof(Player));

		fout.close();
		fin.close();

		remove("records.bin");
		rename("new_records.bin", "records.bin");
	}

	void writeInLastGame()
	{
		ofstream fout("last_game.bin", ios_base::out | ios_base::trunc);

		fout.write((char*)&player, sizeof(Player));
		fout.write((char*)&cellCount, sizeof(int));
		fout.write((char*)&bestScore, sizeof(int));
		fout.write((char*)&array, sizeof(array));

		fout.close();
	}

	void drawGameOver()
	{
		SDL_Delay(500);

		for (int resolution = cellResolution; resolution > cellResolution / 4; resolution -= cellResolution / 32)
		{
			SDL_RenderClear(renderer);

			SDL_Rect cell_in_file = { 0, 0, resolutionInFile, resolutionInFile };
			SDL_Rect texture_position = { widthShift, heightShift, fieldWidth, fieldHeight };

			SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

			texture_position.w = cellResolution;
			texture_position.h = cellResolution;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (i == 1)
					{
						texture_position.y = i * cellResolution + cellHeightShift + (cellResolution - resolution) / 2;
						texture_position.x = j * cellResolution + cellWidthShift + (cellResolution - resolution) / 2;
						texture_position.w = texture_position.h = resolution;
						cell_in_file.x = resolutionInFile * powerOfTwo(array[i][j]);
						SDL_RenderCopy(renderer, cellsTexture, &cell_in_file, &texture_position);
					}
					else
					{
						texture_position.y = i * cellResolution + cellHeightShift;
						texture_position.x = j * cellResolution + cellWidthShift;
						texture_position.w = cellResolution;
						texture_position.h = cellResolution;
						cell_in_file.x = resolutionInFile * powerOfTwo(array[i][j]);
						SDL_RenderCopy(renderer, cellsTexture, &cell_in_file, &texture_position);
					}
				}
			}
			renderScore(player.score, false);
			renderScore(bestScore, true);
			SDL_RenderPresent(renderer);
		}

		for (int j = 0; j < 4; j++)
			array[1][j] = 0;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
		renderCells();
		renderScore(player.score, false);
		renderScore(bestScore, true);
		SDL_RenderPresent(renderer);

		int max_h = 62 * fieldHeight / 322;
		SDL_Rect back;
		back.w = fieldWidth * 5 / 7;
		back.x = backgroundPosition.x + ((fieldWidth - back.w) / 2);
		for (back.h = 2; back.h < max_h; back.h += max_h / 32)
		{
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
			back.y = backgroundPosition.y + ((fieldHeight - back.h) / 2);
			renderCells();
			SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);
			SDL_RenderFillRect(renderer, &back);
			renderScore(player.score, false);
			renderScore(bestScore, true);
			SDL_SetRenderDrawColor(renderer, 117, 117, 117, 255);
			SDL_RenderPresent(renderer);
		}
		back.h = max_h;
		back.y = backgroundPosition.y + ((fieldHeight - back.h) / 2);

		SDL_Texture* game_over;
		SDL_Rect game_over_in_file = { 0, 0, 258, 322 };

		surface = IMG_Load("textures/game_over.png");
		game_over = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);
		SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);
		SDL_RenderFillRect(renderer, &back);
		SDL_SetRenderDrawColor(renderer, 117, 117, 117, 255);
		SDL_RenderCopy(renderer, game_over, &game_over_in_file, &backgroundPosition);
		renderScore(player.score, false);
		renderScore(bestScore, true);
		renderCells();

		SDL_RenderPresent(renderer);

		SDL_Delay(1500);
		SDL_DestroyTexture(game_over);
	}

public:

	Field(SDL_Renderer* renderer, int screenW, int screenH, char* name, int nameLength)
	{
		cellCount = 0;
		player.score = 0;
		this->renderer = renderer;
		backgroundInFile = { 0, 0, 256, 320 };

		surface = IMG_Load("textures/new_field.png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/cells.png");
		cellsTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/numbers.png");
		numbersTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		ifstream last_game("last_game.bin", ios::binary);
		if (last_game.is_open())
		{
			Player prev_player;
			last_game.read((char*)&prev_player, sizeof(Player));
			writeInRecords(prev_player);
			last_game.close();
			remove("last_game.bin");
		}

		ifstream fin("records.bin", ios::binary);
		if (fin.is_open())
		{
			if (fin.peek() == EOF)
				bestScore = 0;
			else
			{
				Player best_player;
				fin.read((char*)&best_player, sizeof(Player));
				bestScore = best_player.score;
			}
			fin.close();
		}
		else
			bestScore = 0;

		for (int i = 0; i < nameLength; i++)
			player.name[i] = name[i];

		setWindowResolution(screenW, screenH);

		addCell();
		addCell();
	}

	Field(SDL_Renderer* renderer, int screenW, int screenHeight)
	{
		backgroundInFile = { 0, 0, 256, 320 };
		ifstream fin("last_game.bin"/*, ios::binary*/);

		fin.read((char*)&player, sizeof(Player));
		fin.read((char*)&cellCount, sizeof(int));
		fin.read((char*)&bestScore, sizeof(int));
		fin.read((char*)&array, sizeof(array));

		fin.close();

		remove("last_game.bin");
		this->renderer = renderer;
		background = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/new_field.png"));
		cellsTexture = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/cells.png"));
		numbersTexture = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/numbers.png"));

		setWindowResolution(screenW, screenHeight);
	}

	~Field()
	{
		if (cellCount == 16)
			writeInRecords(player);
		else
			writeInLastGame();

		SDL_DestroyTexture(background);
		SDL_DestroyTexture(cellsTexture);
		SDL_DestroyTexture(numbersTexture);
	}

	void setWindowResolution(int width, int height) override
	{
		int originalW = 258;
		int originalH = 322;

		if (width < originalW || height < originalH)
		{
			width = fieldWidth = originalW;
			height = fieldHeight = originalH;

			SDL_SetWindowSize(gameInitializer.window, fieldWidth, fieldHeight);
		}
		else if (width <= height)
		{
			fieldHeight = height;
			fieldWidth = originalW * (double)fieldHeight / originalH;
			while (fieldWidth > width)
			{
				fieldHeight--;
				fieldWidth = originalW * (double)fieldHeight / originalH;
			}
		}
		else
		{
			fieldWidth = width;
			fieldHeight = originalH * (double)fieldWidth / originalW;
			while (fieldHeight > height)
			{
				fieldWidth--;
				fieldHeight = originalH * (double)fieldWidth / originalW;
			}
		}

		widthShift = (width - fieldWidth) / 2;
		heightShift = (height - fieldHeight) / 2;

		cellResolution = (fieldWidth - (2.0 * fieldWidth / originalW)) / 4.0;
		cellWidthShift = widthShift;
		cellHeightShift = heightShift + cellResolution;

		backgroundPosition = { widthShift, heightShift, 4 * cellResolution, 5 * cellResolution };

		numberHeight = 14.0 * fieldHeight / originalH;
		numberWidth = 11 * fieldWidth / originalW;
		numberOneWidth = 8 * fieldWidth / originalW;
		numberHeigthShift = heightShift + cellResolution / 2 + 7 * fieldHeight / originalH;

		drawField();
	}

	bool left()
	{
		bool haveUpdate = false;
		resetAdditionalArrays();

		for (int column = 1; column < 4; column++)
		{
			for (int line = 0; line < 4; line++)
			{
				int columnCopy = column;

				while (columnCopy > 0 && array[line][columnCopy - 1] == 0 && array[line][columnCopy] != 0)
				{
					haveUpdate = true;
					array[line][columnCopy - 1] = array[line][columnCopy];
					array[line][columnCopy] = 0;
					array[line][column].shift++;
					columnCopy--;
				}

				if (columnCopy > 0 && array[line][columnCopy - 1] == array[line][columnCopy] && array[line][columnCopy] != 0
					&& array[line][columnCopy - 1].isNew == false && array[line][columnCopy].isNew == false)
				{
					haveUpdate = true;
					array[line][columnCopy - 1] = 2 * array[line][columnCopy];
					array[line][columnCopy - 1].isNew = true;
					array[line][columnCopy] = 0;
					array[line][column].shift++;
					cellCount--;
					player.score += array[line][columnCopy - 1];
				}
			}
		}
		if (player.score > bestScore)
			bestScore = player.score;

		drawHorizontalOffset(toLeft);

		if (haveUpdate == true)
		{
			if (addCell() == false)
			{
				drawField();
				return false;
			}
			else
			{
				drawGameOver();
				return true;
			}
		}
		else
			return false;
	}

	bool right()
	{
		bool haveUpdate = false;
		resetAdditionalArrays();

		for (int column = 2; column > -1; column--)
		{
			for (int line = 0; line < 4; line++)
			{
				int columnCopy = column;

				while (columnCopy < 3 && array[line][columnCopy + 1] == 0 && array[line][columnCopy] != 0)
				{
					haveUpdate = true;
					array[line][columnCopy + 1] = array[line][columnCopy];
					array[line][columnCopy] = 0;
					array[line][column].shift++;
					columnCopy++;
				}

				if (columnCopy < 3 && array[line][columnCopy + 1] == array[line][columnCopy] && array[line][columnCopy] != 0
					&& array[line][columnCopy + 1].isNew == false && array[line][columnCopy].isNew == false)
				{
					haveUpdate = true;
					array[line][columnCopy + 1] = 2 * array[line][columnCopy];
					array[line][columnCopy + 1].isNew = true;
					array[line][columnCopy] = 0;
					array[line][column].shift++;
					cellCount--;
					player.score += array[line][columnCopy + 1];
				}
			}
		}
		if (player.score > bestScore)
			bestScore = player.score;

		drawHorizontalOffset(toRight);

		if (haveUpdate == true)
		{
			if (addCell() == false)
			{
				drawField();
				return false;
			}
			else
			{
				drawGameOver();
				return true;
			}
		}
		else
			return false;
	}

	bool down()
	{
		bool haveUpdate = false;
		resetAdditionalArrays();

		for (int line = 2; line > -1; line--)
		{
			for (int column = 0; column < 4; column++)
			{
				int lineCopy = line;

				while (lineCopy < 3 && array[lineCopy + 1][column] == 0 && array[lineCopy][column] != 0)
				{
					haveUpdate = true;
					array[lineCopy + 1][column] = array[lineCopy][column];
					array[lineCopy][column] = 0;
					array[line][column].shift++;
					lineCopy++;
				}

				if (lineCopy < 3 && array[lineCopy + 1][column] == array[lineCopy][column] && array[lineCopy][column] != 0
					&& array[lineCopy + 1][column].isNew == false && array[lineCopy][column].isNew == false)
				{
					haveUpdate = true;
					array[lineCopy + 1][column] = 2 * array[lineCopy][column];
					array[lineCopy + 1][column].isNew = true;
					array[lineCopy][column] = 0;
					array[line][column].shift++;
					cellCount--;
					player.score += array[lineCopy + 1][column];
				}
			}
		}
		if (player.score > bestScore)
			bestScore = player.score;

		drawVerticalOffset(toDown);

		if (haveUpdate == true)
		{
			if (addCell() == false)
			{
				drawField();
				return false;
			}
			else
			{
				drawGameOver();
				return true;
			}
		}
		else
			return false;
	}

	bool up()
	{
		bool haveUpdate = false;
		resetAdditionalArrays();

		for (int line = 1; line < 4; line++)
		{
			for (int column = 0; column < 4; column++)
			{
				int lineCopy = line;

				while (lineCopy > 0 && array[lineCopy - 1][column] == 0 && array[lineCopy][column] != 0)
				{
					haveUpdate = true;
					array[lineCopy - 1][column] = array[lineCopy][column];
					array[lineCopy][column] = 0;
					array[line][column].shift++;
					lineCopy--;
				}

				if (lineCopy > 0 && array[lineCopy - 1][column] == array[lineCopy][column] && array[lineCopy][column] != 0
					&& array[lineCopy - 1][column].isNew == false && array[lineCopy][column].isNew == false)
				{
					haveUpdate = true;
					array[lineCopy - 1][column] = 2 * array[lineCopy][column];
					array[lineCopy - 1][column].isNew = true;
					array[lineCopy][column] = 0;
					array[line][column].shift++;
					cellCount--;
					player.score += array[lineCopy - 1][column];
				}
			}
		}
		if (player.score > bestScore)
			bestScore = player.score;

		drawVerticalOffset(toUp);

		if (haveUpdate == true)
		{
			if (addCell() == false)
			{
				drawField();
				return false;
			}
			else
			{
				drawGameOver();
				return true;
			}
		}
		else
			return false;
	}
};