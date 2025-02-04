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
	void Update(float dt);
	virtual void Draw(LineRenderer* lines) const = 0;
	virtual float CalculateMass();

	void SetVelocity(Vec2 force) { velocity = force; }
	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 force);
	Vec2 GetVelocity() { return velocity; }
	Vec2 GetVelocityNormalised() { return velocity.GetNormalised(); }

	unsigned int GUID;
	PHYSICSTYPE GetType() { return type; }
	float elasticity;
private:
protected:
	PHYSICSTYPE type;
	Vec2 position, velocity, accumulatedForce;
	float inverseMass;

	float orientation;
	float angularVelocity;
	float momentOfIntertia;

};