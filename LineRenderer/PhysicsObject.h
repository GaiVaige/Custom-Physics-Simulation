#pragma once
#include "Vec2.h"
#include "Collider.h"
class LineRenderer;

enum PHYSICSTYPE {
	STATIC,
	DYNAMIC,
	TRIGGER
};

class PhysicsObject {
public:

	PhysicsObject();
	~PhysicsObject();
	PhysicsObject(PhysicsObject& po) = delete;
	PhysicsObject& operator=(PhysicsObject& po) = delete;

	Collider* collider = nullptr;
	const Vec2 GetPos() const { return position; }
	void SetPosition(Vec2& v);
	void OffsetPosition(Vec2& v);
	virtual void Draw(LineRenderer* lines) const = 0;

	unsigned int GUID;
	PHYSICSTYPE GetType() { return type; }
private:
protected:
	PHYSICSTYPE type;
	Vec2 position, velocity;
	float mass;

};