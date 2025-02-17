#pragma once
#include "Polygon.h"
#include <array>

struct Wheel {
	Wheel(Vec2 pos) : pos(pos) {};
	Vec2 pos;
	float rotSpeed;
};

static std::vector<Vec2> carVerts{
	{-1, -2},
	{-1, 2},
	{1, 2},
	{1, -2}
};

static std::vector<Vec2> wheelVerts{
	{-.1, -.3},
	{-.1, .3},
	{.1, .3},
	{.1, -.3}
};

class Car : public Polygon {

public:
	Car(Vec2 pos);
	void CalculateVelocityAdditions();
	void Accelerate(float throttleAmnt);
	void Brake();
	void Draw(LineRenderer* lines) const override;
private:
	int gear;
	
	float brakeStrength;

	float currentThrottle;
	float currentTurningAmount;
	std::vector<Wheel> wheels;
};