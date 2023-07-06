#pragma once
#include <map>
#include <string>
#include <SDL.h>

class IProgramState;

class Program
{
public:
	enum EnState
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

private:

	EnState mState;
	std::map<EnState, IProgramState*> mProgramStatesMap;

public:
	Program();

	bool Initialize(std::string& sProblem);
	void SetState(const EnState eState);
	void Update(const SDL_Event& event);
	void Draw();
};
