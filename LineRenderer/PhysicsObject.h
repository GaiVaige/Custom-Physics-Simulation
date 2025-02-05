#pragma once
#include "Vec2.h"
#include "Collider.h"
class LineRenderer;

static float LINEAR_THRESHOLD = .05f;
static Vec2 GRAVITY = Vec2(0, -9.8);

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
	float CalculateMomentOfInertia(Vec2 centreOfMass, std::vector<Vec2>& points, float pointWeight);

	virtual void Rotate(float amnt);

	void SetVelocity(Vec2 force) { linearVelocity = force; }
	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 force);
	Vec2 GetVelocity() { return linearVelocity; }
	Vec2 GetVelocityNormalised() { return linearVelocity.GetNormalised(); }

	unsigned int GUID;
	PHYSICSTYPE GetType() { return type; }
	float elasticity;

	float linearDrag = .5;
	float angularDrag = .01;

private:
protected:
	PHYSICSTYPE type;
	Vec2 position, linearVelocity, accumulatedLinearForce;
	float inverseMass;

	float orientation;
	float angularVelocity;
	float accumulatedAngularForce;
	float momentOfIntertia;

};