#pragma once

struct Vec3
{
	int mX;
	int mY;
	int mZ;

	Vec3() : mX(0), mY(0), mZ(0) { }
	Vec3(int x, int y, int z) : mX(x), mY(y), mZ(z) { }
};
