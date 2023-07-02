#pragma once
#include <Game/ProgramStates/ProgramState.h>
#include <Renderer/Sprite.h>

class GameIntro final : public IProgramState
{
private:
	Sprite* mpBackground;

public:
	GameIntro(Sprite* pBackground) : mpBackground(pBackground)
	{

	}

	void Update(const SDL_Event& event) override { }
	void Draw() override 
	{

	}
};
