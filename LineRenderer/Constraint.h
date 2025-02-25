#pragma once
#include "Vec2.h"
class PhysicsObject;
struct Constraint {
public:
	virtual void Constrain(PhysicsObject* object) = 0;
};

struct PinConstraint : public Constraint {
public:
	PinConstraint() {};
	PinConstraint(Vec2 pos) : position(pos) {};
	void Constrain(PhysicsObject* object) override;
private:
	Vec2 position = Vec2(0,0);
};