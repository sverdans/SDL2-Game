#pragma once
#include <SDL.h>

class Program;

class IProgramState
{
protected:
    Program* mpProgram;

public:
    IProgramState(Program* pProgram);
    virtual ~IProgramState();

    virtual bool Initialize() = 0;
    virtual void Draw() = 0;
    virtual void Update(const SDL_Event& event) = 0;
};