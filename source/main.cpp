#include <iostream>
#include <filesystem>
#include <vector>
#include <tuple>

#include <System/Resources.h>
#include <System/Window.h>
#include <Game/Program.h>

inline void Quit(int code)
{
	Resources::Instance().Clear();
	Window::Instance().Finalize();
	exit(code);
}

int main(int argc, char** argv)
{
	std::string sProblem;
	bool lShouldClose { false };

//--Initializing----------------------------------------------------------------------------------------------------
	if (!Window::Instance().Initialize("2048", { 516, 644 }, sProblem))
	{
		std::cout << "Error: " << sProblem << std::endl;
		Quit(-1);
	}
	
//--Loading-Resources-----------------------------------------------------------------------------------------------
	{
		std::filesystem::path path(argv[0]);
		std::string sResourcesDir = path.parent_path().parent_path().string() + "/resources/";
		Resources::Instance().SetRootDir(sResourcesDir);
		
		std::vector<std::pair<std::string, std::string>> textures = {
			{ "GameIntroBackground", "petrov.a.e..png" },
		};

		for (const auto& [sTextureName, sFilePath] : textures)
		{
			if (!Resources::Instance().LoadTexture(sTextureName, sFilePath, sProblem))
			{
				std::cout << "Error: " << sProblem << std::endl;
				Quit(-1);
			}
		}

		std::vector<std::tuple<std::string, std::string, SDL_Rect>> sprites = {
			{ "GameIntroBackground", "GameIntroBackground", { 0, 0, 300, 300 } },
		};

		for (const auto& [sSpriteName, sTextureName, srcRect] : sprites)
		{
			if (!Resources::Instance().LoadSprite(sSpriteName, sTextureName, srcRect, sProblem))
			{
				std::cout << "Error: " << sProblem << std::endl;
				Quit(-1);
			}
		}
	}

	Program program;
	if (!program.Initialize(sProblem))
	{
		std::cout << "Error: " << sProblem << std::endl;
		Quit(-1);
	}

	Window::Instance().SetClearColor(117, 117, 117);

//--Program-Loop----------------------------------------------------------------------------------------------------
	while (!lShouldClose)
	{
		Window::Instance().Clear();

		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
			{
				lShouldClose = true;
				break;
			}

			program.Update(event);
		}

		program.Draw();
		
		Window::Instance().Render();
	}

	Quit(0);
}