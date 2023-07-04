#pragma once
#include <map>

#include <SDL.h>

#include <Game/ProgramStates/ProgramState.h>
#include <Game/ProgramStates/GameIntro.h>
#include <Resources/ResourceManager.h>

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
			return false;

		IProgramState* pGameIntro = new GameIntro(pGameIntroBackground);
		return true;	
	}

	void Update(const SDL_Event& event)
	{

	}

	void Draw()
	{

	}
};
