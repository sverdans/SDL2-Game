#include <Game/Program.h>
#include <Game/IProgramState.h>
#include <Game/ProgramStates/GameIntro.h>
#include <Resources/ResourceManager.h>


Program::Program() : mState(EnState::eInIntro) 
{
	mProgramStatesMap[EnState::eInIntro] = new GameIntro(this);
}

bool Program::Initialize() 
{
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
