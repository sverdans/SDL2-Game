#include <Game/Program.h>
#include <Game/IProgramState.h>
#include <Game/ProgramStates/GameIntro.h>
#include <System/Resources.h>

Program::Program() : mState(EnState::eInIntro) 
{
	mProgramStatesMap[EnState::eInIntro] = new GameIntro(this);
}

bool Program::Initialize(std::string& sProblem) 
{
	for (const auto& [eState, pProgramState] : mProgramStatesMap)
	{
		if (!pProgramState->Initialize())
		{
			sProblem = "ProgramState with ID: " + std::to_string(eState) + " can not be initialized";
			return false;
		}
	}
	return true;
}

void Program::SetState(const EnState eState)
{
	mState = eState;
}

void Program::Update(const SDL_Event& event)
{
	mProgramStatesMap[mState]->Update(event);
}

void Program::Draw()
{
	mProgramStatesMap[mState]->Draw();
}
