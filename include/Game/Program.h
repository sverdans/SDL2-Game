#pragma once
#include <map>

#include <SDL.h>

class ProgramState;

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
	std::map<EnState, ProgramState*> mPorgamStateMap;
public:

	Program() : meState(EnState::eInMenu) { }

	void Update(const SDL_Event& event)
	{

	}
};
