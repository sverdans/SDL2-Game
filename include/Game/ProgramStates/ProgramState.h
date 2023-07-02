#pragma once
#include <SDL.h>

class IProgramState
{
public:
    virtual ~IProgramState() { }

    virtual void Draw() = 0;
    virtual void Update(const SDL_Event& event) = 0;
};