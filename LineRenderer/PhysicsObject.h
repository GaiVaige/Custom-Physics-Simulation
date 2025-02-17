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
	const float GetOrientation() const { return orientation; }
	void SetPosition(Vec2& v);
	void OffsetPosition(Vec2& v);
	void Update(float dt);
	virtual void Draw(LineRenderer* lines) const = 0;
	virtual float CalculateMass();
	float CalculateMomentOfInertia(Vec2 centreOfMass, std::vector<Vec2>& points, float pointWeight);
	void DrawOrientingAxes(LineRenderer* lines) const;
	virtual void Rotate(float amnt);

	void SetVelocity(Vec2 force) { linearVelocity = force; }
	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 force);
	Vec2 GetVelocity() const { return linearVelocity; }
	float GetAngularVelocity() { return angularVelocity; }
	Vec2 GetVelocityNormalised() { return linearVelocity.GetNormalised(); }

	void ApplyAngularForce(Vec2 force, Vec2 pos);
	void ApplyForceAt(Vec2 force, Vec2 pos);
	Vec2 GetVelocityAt(Vec2 pos) const;
	void SetMOI(float f) { inverseMomentOfInertia = f; }

	unsigned int GUID;
	PHYSICSTYPE GetType() { return type; }
	float elasticity;

	float linearDrag = .5;
	float angularDrag = .3;
	float inverseMomentOfInertia;

private:
protected:
	PHYSICSTYPE type;
	Vec2 position, linearVelocity, accumulatedLinearForce, centreOfMassDisplacement;
	Vec2 up, right;
	float inverseMass;

	float orientation;
	float angularVelocity;
	float accumulatedAngularForce;

};