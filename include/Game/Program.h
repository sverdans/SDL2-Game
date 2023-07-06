#pragma once
#include <map>

#include <SDL.h>

#include <Game/ProgramStates/ProgramState.h>
#include <Game/ProgramStates/GameIntro.h>
#include <Resources/ResourceManager.h>

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

	Program() : mState(EnState::eInIntro) 
	{
		mProgramStatesMap[EnState::eInIntro] = new GameIntro(this);
	}

	void AddState(const EnState eState, IProgramState* pProgramState)
	{
		if (mProgramStatesMap.find(eState) != mProgramStatesMap.end())
			return;

		mProgramStatesMap[eState] = pProgramState;
	}

	void SetState(const EnState eState)
	{
		mState = eState;
	}

	void Update(const SDL_Event& event)
	{
		mProgramStatesMap[mState]->Update(event);
	}

	void Draw()
	{
		mProgramStatesMap[mState]->Draw();
	}
};
