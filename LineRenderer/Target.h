#pragma once
#include "Circle.h"
class Launcher;
class Target : public Circle {
public:
	Target(Vec2 pos) : Circle(pos, .9, 1) { inverseMass = 0; collider->SetInvMass(0); inverseMomentOfInertia = 0; };

	void CollisionEvent(PhysicsObject* other) override;
	void Draw(LineRenderer* lines) const override;
};

class FrenzyTarget : public Target {

public:
	FrenzyTarget(Vec2 pos, int spawnCount, Vec2 spawnPos, std::vector<PhysicsObject*>& queue)
		: Target(pos), spawnCount(spawnCount), obQueue(queue), spawnPos(spawnPos) {};

	void CollisionEvent(PhysicsObject* other) override;
	void Draw(LineRenderer* lines) const override;
private:
	std::vector<PhysicsObject*>& obQueue;
	int spawnCount = 0;
	Vec2 spawnPos;
};