#include "Car.h"
#include "LineRenderer.h"
Car::Car(Vec2 pos)
	: Polygon(pos, carVerts, .5)
{
	wheels.push_back(Wheel(Vec2(- 1, -1.5)));
	wheels.push_back(Wheel(Vec2(-1, 1.5)));
	wheels.push_back(Wheel(Vec2(1, 1.5)));
	wheels.push_back(Wheel(Vec2(1, -1.5)));

}

void Car::Draw(LineRenderer* lines) const
{
	for (Wheel w : wheels) {
		for (int i = 0; i < 4; i++) {
			lines->AddPointToLine(w.pos + wheelVerts[i]);
		}
		lines->FinishLineLoop();
	}
	Polygon::Draw(lines);
}

