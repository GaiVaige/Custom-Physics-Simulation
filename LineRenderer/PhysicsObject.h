#pragma once
#include "Vec2.h"
#include "Collider.h"
class LineRenderer;
class PhysicsObject {
public:

	PhysicsObject();
	~PhysicsObject();
	PhysicsObject(PhysicsObject& po) = delete;
	PhysicsObject& operator=(PhysicsObject& po) = delete;

	Collider* collider = nullptr;
	Vec2 GetPos() { return position; }
	void SetPosition(Vec2& v) { position = v; }
	virtual void Draw(LineRenderer* lines) {};

	unsigned int GUID;
private:
protected:
	Vec2 position, velocity;
	float mass;
};