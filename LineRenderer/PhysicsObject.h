#pragma once
#include "Vec2.h"
#include "Collider.h"
class LineRenderer;
class Constraint;
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
	virtual ~PhysicsObject();
	PhysicsObject(PhysicsObject& po) = delete;
	PhysicsObject& operator=(PhysicsObject& po) = delete;

	Collider* collider = nullptr;
	virtual void Draw(LineRenderer* lines) const = 0;
	virtual float CalculateMass();
	virtual void Rotate(float amnt);
	void RotateAbout(float amnt, Vec2 pos);
	const Vec2 GetPos() const { return position; }
	const float GetOrientation() const { return orientation; }
	void SetPosition(Vec2& v);
	void OffsetPosition(Vec2& v);
	virtual void Update(float dt);
	float CalculateMomentOfInertia(Vec2 centreOfMass, std::vector<Vec2>& points, float pointWeight);

	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 force);
	Vec2 GetVelocity() const { return linearVelocity; }
	void SetVelocity(Vec2 newVel) { linearVelocity = newVel; }
	float GetAngularVelocity() { return angularVelocity; }

	void ApplyAngularForce(Vec2 force, Vec2 pos);
	void ApplyForceAt(Vec2 force, Vec2 pos);
	Vec2 GetVelocityAt(Vec2 pos) const;

	void DrawOrientingAxes(LineRenderer* lines) const;
	
	virtual void Unload();
	virtual void CollisionEvent(PhysicsObject* other = nullptr) {};
	
	PHYSICSTYPE GetType() { return type; }
	bool markedForDeletion = false;
	bool useGravity = false;

	unsigned int GUID;
	float elasticity = 0;
	float linearDrag = .5;
	float angularDrag = .3;
	float angularVelocity = 0;
	float accumulatedAngularForce = 0;
	float inverseMass = 0;
	float inverseMomentOfInertia = 0;
	float orientation = 0;

	Vec2 up, right, accumulatedLinearForce;
	Constraint* constraint = nullptr;

	bool constraintResolvedRotation = false;
	bool constraintResolvedPosition = false;
private:
protected:
	PHYSICSTYPE type;
	Vec2 position, linearVelocity, centreOfMassDisplacement;


};