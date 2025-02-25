#pragma once
#include "Circle.h"
class Launcher;
class Target : public Circle {
public:
	Target(Vec2 pos, Launcher* player) : Circle(pos, .9, 1), player(player) {};

	Launcher* player;

	void Notify(PhysicsObject* other) override;
	void Draw(LineRenderer* lines) const override;
};