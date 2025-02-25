#pragma once
#include "Circle.h"
class Launcher;
class Target : public Circle {
public:
	Target(Vec2 pos) : Circle(pos, .9, 1) { inverseMass = 0; collider->SetInvMass(0); inverseMomentOfInertia = 0; };

	void Notify(PhysicsObject* other) override;
	void Draw(LineRenderer* lines) const override;
};