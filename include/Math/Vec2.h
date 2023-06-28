#pragma once

struct Vec2
{
    int mX;
    int mY;

    Vec2() : mX(0), mY(0) { }
    Vec2(int x) : mX(x), mY(x) { }
    Vec2(int x, int y) : mX(x), mY(y) { }
};
