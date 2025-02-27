#pragma once
#include "Vec2.h"
class PhysicsObject;
struct Constraint {
public:
	virtual void Constrain(PhysicsObject* object, float dt = 0) = 0;

	Vec2 position = Vec2(0, 0);
};

struct PinConstraint : public Constraint {
public:
	PinConstraint(Vec2 pos) { position = pos; };
	virtual void Constrain(PhysicsObject* object, float dt = 0) override;
};

struct SpringConstraint : public Constraint {
public:
	SpringConstraint() {};
	//Strength should be a range between 0 and 1
	SpringConstraint(Vec2 pos, float strength) : springStrength(strength) { position = pos; };

	void Constrain(PhysicsObject* object, float dt = 0) override;
private:
	float springStrength = 0;
};

struct Joint : public PinConstraint{
public:
	Joint(Vec2 pos) : PinConstraint(pos) {};

	virtual void Constrain(PhysicsObject* object, float dt = 0) = 0;
	void Attach(PhysicsObject* object);
};

struct HingeJoint : public Joint {
public:
	HingeJoint(Vec2 pos) : Joint(pos) {};
	void Constrain(PhysicsObject* object, float dt) override;
};