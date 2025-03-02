#pragma once
#include "Vec2.h"
#include "Collider.h"
class LineRenderer;
class Constraint;
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

	//General/Linear Physics
	virtual float CalculateMass();
	const Vec2 GetPos() const { return position; }

	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 force);
	Vec2 GetVelocity() const { return linearVelocity; }
	void SetVelocity(Vec2 newVel) { linearVelocity = newVel; }

	float inverseMass = 0;
	float linearDrag = .5;

	//Rotational 
	float CalculateMomentOfInertia(Vec2 centreOfMass, std::vector<Vec2>& points, float pointWeight);
	virtual void Rotate(float amnt);
	void RotateAbout(float amnt, Vec2 pos);
	const float GetOrientation() const { return orientation; }
	
	void ApplyForceAt(Vec2 force, Vec2 pos);
	void ApplyAngularForce(Vec2 force, Vec2 pos);
	float GetAngularVelocity() { return angularVelocity; }
	Vec2 GetVelocityAt(Vec2 pos) const;
	
	float angularVelocity = 0;
	float inverseMomentOfInertia = 0;
	float accumulatedAngularForce = 0;
	float angularDrag = .3;
	float orientation = 0;

	//Engine and Collision 
	Collider* collider = nullptr;
	Constraint* constraint = nullptr;
	bool constraintResolvedRotation = false;
	bool constraintResolvedPosition = false;
	
	unsigned int GUID;
	bool markedForDeletion = false;
	bool useGravity = false;
	
	float elasticity = 0;

	PHYSICSTYPE GetType() { return type; }
	virtual void Update(float dt);
	virtual void Draw(LineRenderer* lines) const = 0;
	void SetPosition(Vec2& v);
	void OffsetPosition(Vec2& v);
	
	virtual void CollisionEvent(PhysicsObject* other = nullptr) {};

	Vec2 up, right, accumulatedLinearForce;
	//Debug Functions
	void DrawOrientingAxes(LineRenderer* lines) const;

	virtual void Unload();

private:
protected:
	PHYSICSTYPE type;
	Vec2 position, linearVelocity, centreOfMassDisplacement;


};