#pragma once
#include "Vec2.h"
#include "Collider.h"

class PhysicsObject {
public:

	PhysicsObject();
	~PhysicsObject();
	PhysicsObject(PhysicsObject& po) = delete;
	PhysicsObject& operator=(PhysicsObject& po) = delete;

	Collider* collider = nullptr;
	unsigned int GUID;
private:
protected:
	Vec2 position, velocity;
	float mass;
};