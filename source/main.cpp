#include <iostream>

#include <System/Window.h>
#include <Renderer/Renderer.h>
#include <Game/Program.h>

#include <GameInitializer.h>
#include <NameEntering.h>
#include <game_records.h>
#include <Field.h>
#include <manual.h>
#include <Menu.h>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

bool event_handler(SDL_Event* game_event)
{
	bool game_exit = false;
	bool end_of_game = false;
	char* name = nullptr;
	const int name_length = 13;
	int mouse_x, mouse_y;

	eMenuItem game_position = inIntro;

	Intro* splash_screen = new Intro(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
	Field* matrix = nullptr;
	Menu* game_menu = nullptr;
	GameOverMenu* menu_in_game_over = nullptr;
	NameEntering* name_enter = nullptr;
	game_records* records = nullptr;
	manual* instruction = nullptr;
	SDL_Rect letter;

	while (SDL_WaitEvent(game_event) != 0 && game_exit == false)
	{
		switch (game_position)
		{
		case inIntro:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				game_exit = true;
				break;

			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					splash_screen->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (game_event->button.button)
				{
				case SDL_BUTTON_LEFT:
					delete splash_screen;
					game_menu = new Menu(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					game_position = inMenu;
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (game_event->key.keysym.sym)
				{
				case SDLK_RETURN:
					delete splash_screen;
					game_menu = new Menu(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					game_position = inMenu;
					break;
				}
			}
			break;
		}

		case inMenu:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				game_exit = true;
				break;

			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;

			case SDL_MOUSEMOTION:
			{
				mouse_x = game_event->motion.x;
				mouse_y = game_event->motion.y;
				game_menu->mouseInMenu(mouse_x, mouse_y);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				switch (game_event->button.button)
				{
				case SDL_BUTTON_LEFT:
					game_position = game_menu->enter(true, mouse_x, mouse_y);
					if (game_position == newGame)
						name_enter = new NameEntering(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height, name_length);
					else if (game_position == inRecords)
						records = new game_records(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					else if (game_position == howToPlay)
						instruction = new manual(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					break;
				}
				break;
			}
			case SDL_KEYDOWN:
				switch (game_event->key.keysym.sym)
				{
				case SDLK_UP:
					game_menu->up(false);
					break;
				case SDLK_DOWN:
					game_menu->down(false);
					break;
				case SDLK_RETURN:
					game_position = game_menu->enter(false, 0, 0);
					if (game_position == newGame)
						name_enter = new NameEntering(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height, name_length);
					else if (game_position == inRecords)
						records = new game_records(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					else if (game_position == howToPlay)
						instruction = new manual(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					break;
				}
			}
			break;
		}

		case continueGame:
		{
			if (matrix != nullptr)
			{
				matrix->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				game_position = inGame;
			}
			else
			{
				ifstream last_game("last_game.bin");
				if (last_game.is_open())
				{
					last_game.close();
					matrix = new Field(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					game_position = inGame;
				}
				else
					game_position = inMenu;
			}
			break;
		}

		case newGame:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				delete name_enter;
				game_exit = true;
				break;

			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					name_enter->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;
			case SDL_KEYDOWN:
			{
				auto option = game_event->key.keysym.sym;
				if (option == SDLK_ESCAPE)
				{
					delete name_enter;
					game_position = inMenu;
					game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				else if (option == SDLK_RETURN)
				{
					if (name_enter->get_name(&name) == true)
					{
						delete name_enter;
						if (matrix != nullptr)
							delete matrix;

						matrix = new Field(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height, name, name_length);
						game_position = inGame;
					}
				}
				else if (option == SDLK_LEFT)
					name_enter->cursor_left();

				else if (option == SDLK_RIGHT)
					name_enter->cursor_right();

				else if (option == SDLK_SPACE)
					name_enter->add_letter(letter = { 338, 0, 11, 14 }, ' ');

				else if (option >= SDLK_a && option <= SDLK_z)
				{
					letter = { (option - 97) * 13, 0, 11, 14 };
					if (option == SDLK_i)
						letter.w = 5;
					if (option == SDLK_q)
						letter.w = 13;
					name_enter->add_letter(letter, char(option));
				}
				else if (option == SDLK_BACKSPACE)
					name_enter->delete_letter();
				break;
			}
			}
			break;
		}

		case inGame:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				game_exit = true;
				break;

			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					matrix->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;

			case SDL_KEYDOWN:
				switch (game_event->key.keysym.sym)
				{
				case SDLK_UP:
					end_of_game = matrix->up();
					break;
				case SDLK_DOWN:
					end_of_game = matrix->down();
					break;
				case SDLK_RIGHT:
					end_of_game = matrix->right();
					break;
				case SDLK_LEFT:
					end_of_game = matrix->left();
					break;
				case SDLK_ESCAPE:
					game_position = inMenu;
					game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
					break;
				}

				if (end_of_game == true)
				{
					delete matrix;
					matrix = nullptr;
					game_position = gameOver;
					menu_in_game_over = new GameOverMenu(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height);
					end_of_game = false;
				}
				break;
			}
			break;
		}

		case gameOver:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				delete menu_in_game_over;
				game_exit = true;
				break;

			case SDL_MOUSEMOTION:
			{
				mouse_x = game_event->motion.x;
				mouse_y = game_event->motion.y;
				menu_in_game_over->mouseInMenu(mouse_x, mouse_y);
				break;
			}

			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					menu_in_game_over->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
			{
				switch (game_event->button.button)
				{
				case SDL_BUTTON_LEFT:
					game_position = menu_in_game_over->enter(true, mouse_x, mouse_y);
					if (game_position == newGame)
					{
						delete menu_in_game_over;
						name_enter = new NameEntering(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height, name_length);
					}
					else if (game_position == inMenu)
					{
						delete menu_in_game_over;
						game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
					}
					break;
				}
				break;
			}

			case SDL_KEYDOWN:
				switch (game_event->key.keysym.sym)
				{
				case SDLK_RETURN:
					game_position = menu_in_game_over->enter(false, 0, 0);
					delete menu_in_game_over;
					if (game_position == newGame)
						name_enter = new NameEntering(gameInitializer.renderer, gameInitializer.screen_width, gameInitializer.screen_height, name_length);
					else
						game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
					break;

				case SDLK_ESCAPE:
					delete menu_in_game_over;
					game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
					game_position = inMenu;
					break;

				case SDLK_LEFT:
					menu_in_game_over->left();
					break;

				case SDLK_RIGHT:
					menu_in_game_over->right();
					break;
				}
				break;
			}
			break;
		}

		case inRecords:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				delete records;
				game_exit = true;
				break;
			case SDL_MOUSEWHEEL:
			{
				if (game_event->wheel.y > 0)
					records->prev_record();
				else if (game_event->wheel.y < 0)
					records->next_record();
				break;
			}
			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					records->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;
			case SDL_KEYDOWN:
				switch (game_event->key.keysym.sym)
				{
				case SDLK_ESCAPE:
					delete records;
					game_position = inMenu;
					game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
					break;
				case SDLK_UP:
					records->prev_record();
					break;
				case SDLK_DOWN:
					records->next_record();
					break;
				}
				break;
			}
			break;
		}

		case howToPlay:
		{
			switch (game_event->type)
			{
			case SDL_QUIT:
				delete instruction;
				game_exit = true;
				break;

			case SDL_MOUSEWHEEL:
			{
				if (game_event->wheel.y > 0)
					instruction->up();
				else if (game_event->wheel.y < 0)
					instruction->down();
				break;
			}

			case SDL_WINDOWEVENT:
				if (game_event->window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(gameInitializer.window, &gameInitializer.screen_width, &gameInitializer.screen_height);
					instruction->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
				}
				break;
			case SDL_KEYDOWN:
				switch (game_event->key.keysym.sym)
				{
				case SDLK_ESCAPE:
					delete instruction;
					game_position = inMenu;
					game_menu->setWindowResolution(gameInitializer.screen_width, gameInitializer.screen_height);
					break;
				case SDLK_UP:
					instruction->up();
					break;
				case SDLK_DOWN:
					instruction->down();
					break;
				}
				break;
			}
			break;
		}

		case exitGame:
		{
			game_exit = true;
			break;
		}
		}
	}

	if (game_exit == true)
	{
		if (game_menu != nullptr)
			delete game_menu;
		if (matrix != nullptr)
			delete matrix;
	}

	return game_exit;
}

int main(int argc, char** argv)
{
	std::string sProblem;
	bool lShouldClose { false };

	if (!Window::Instance().Initialize("2048", { 516, 644 }, sProblem))
	{
		std::cout << "Error: " << sProblem << std::endl;
		return -1;
	}

	if (!Renderer::Instance().Initialize(sProblem))
	{
		std::cout << "Error: " << sProblem << std::endl;
		return -1;
	}
	
	Renderer::Instance().SetClearColor(117, 117, 117);
	Program program;

	while (!lShouldClose)
	{
		Renderer::Instance().Clear();

		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
				lShouldClose = true;
			
			program.Update(event);
		}

		program.Draw();
		Renderer::Instance().Render();
	}

	Renderer::Instance().Finalize();
	Window::Instance().Finalize();

	return 0;
}