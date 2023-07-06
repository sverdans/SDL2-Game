#pragma once
#include <Game/IProgramState.h>
#include <Resources/Sprite.h>

class Program;

class GameIntro final : public IProgramState
{
private:
	Sprite* mpBackground;

public:
	GameIntro(Program* pProgram) : IProgramState(pProgram) { }

	bool Initialize() override
	{

		return true;
	}

	void Update(const SDL_Event& event) override 
	{

	}
	
	void Draw() override 
	{

	}
};
