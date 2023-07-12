#pragma once
#include <Game/IProgramState.h>
#include <System/Resources.h>
#include <System/Constants.h>
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
		mpBackground = Resources::Instance().GetSprite("GameIntroBackground");
		if (!mpBackground)
			return false;

		return true;
	}

	void Update(const SDL_Event& event) override 
	{

	}
	
	void Draw() override 
	{
		mpBackground->Render({ 0, 0 }, { CONST::WIDTH, CONST::HEIGHT });
	}
};
