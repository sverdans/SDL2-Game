#pragma once

struct Vec2
{
	int x;
	int y;

	Vec2() : x(0), y(0) { }
	Vec2(int x) : x(x), y(x) { }
	Vec2(int x, int y) : x(x), y(y) { }
};
