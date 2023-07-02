#pragma once
#include <map>

#include <SDL.h>

#include <Game/ProgramStates/ProgramState.h>
#include <Game/ProgramStates/GameIntro.h>
#include <Renderer/ResourceManager.h>

class Program
{
private:
	enum class EnState
	{
		eInIntro,
		eInMenu,
		eInNameEntering,
		eInScoresList,
		eInManual,
		eInGame,
		eInGameOver,
		eInExit,
	};

	EnState meState;
	
public:

	Program() : meState(EnState::eInIntro) 
	{
		

	}

	bool Initialize()
	{
		auto pGameIntroBackground = ResourceManager::Instance().GetSprite("GameIntroBackground");
		if (!pGameIntroBackground)
			return;

		IProgramState* pGameIntro = new GameIntro(pGameIntroBackground);	
	}

	void Update(const SDL_Event& event)
	{

	}

	void Draw()
	{

	}
};
