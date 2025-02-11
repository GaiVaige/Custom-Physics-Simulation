#pragma once
#include "Polygon.h"
#include <array>
class Car : public Polygon {

public:
	void CalculateVelocityAdditions();
	void Accelerate(float throttleAmnt);
	void Brake();
	void ShiftGear(bool up);
private:
	int gear;
	
	float brakeStrength;

	float currentThrottle;
	float currentTurningAmount;

	std::array<std::pair<float, float>, 5> accelAndTurnPairs{
		std::pair<float, float>(-1.f, 1.f),
		std::pair<float, float>(0.f, 1.f),
		std::pair<float, float>(1.f, 1.f),
		std::pair<float, float>(2.f, .5f),
		std::pair<float, float>(4.f, .1f)
	}
};