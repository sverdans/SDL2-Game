#include <iostream>
#include <filesystem>
#include <vector>

#include <Resources/ResourceManager.h>
#include <System/Window.h>
#include <Game/Program.h>

void Quit()
{
	ResourceManager::Instance().Clear();
	Window::Instance().Finalize();
}

int main(int argc, char** argv)
{
	std::string sProblem;
	bool lShouldClose { false };

//--Initializing----------------------------------------------------------------------------------------------------
	if (!Window::Instance().Initialize("2048", { 516, 644 }, sProblem))
	{
		std::cout << "Error: " << sProblem << std::endl;
		Quit();
		return -1;
	}
	
//--Loading Resources-----------------------------------------------------------------------------------------------
	{
		std::filesystem::path path(argv[0]);
		std::string sResourcesDir = path.parent_path().parent_path().string() + "/resources/";
		ResourceManager::Instance().SetRootDir(sResourcesDir);
		
		std::vector<std::pair<std::string, std::string>> texturePaths = {
			{ "GameIntroBackground", "petrov.a.e..png" },
		};

		for (const auto& [sTextureName, sFilePath] : texturePaths)
		{
			if (!ResourceManager::Instance().LoadTexture(sTextureName, sFilePath, sProblem))
			{
				std::cout << "Error: " << sProblem << std::endl;
				Quit();
				return -1;
			}
		}
	}

	Program program;

	Window::Instance().SetClearColor(117, 117, 117);

	while (!lShouldClose)
	{
		Window::Instance().Clear();

		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
				lShouldClose = true;
			
			program.Update(event);
		}

		program.Draw();
		
		Window::Instance().Render();
	}

	Quit();
	return 0;
}