#include "PhysicsObject.h"
#include <chrono>

PhysicsObject::PhysicsObject()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	GUID = nanos;
	linearVelocity = Vec2(0, 0);
}

PhysicsObject::~PhysicsObject()
{
	delete collider;
}

void PhysicsObject::SetPosition(Vec2& v)
{
	position = v; 
	collider->SetPos(position);
}

void PhysicsObject::OffsetPosition(Vec2& v)
{
	position += v;
	collider->SetPos(position);
}

void PhysicsObject::Update(float dt)
{
	Vec2 dragVec = GetVelocityNormalised() * (50 * inverseMass) * dt;
	Vec2 accel = ((accumulatedLinearForce * inverseMass) - (linearVelocity * dt)) - dragVec;
	position += linearVelocity * dt;
	accumulatedLinearForce += accel * dt;
	collider->SetPos(position);
	accumulatedLinearForce = Vec2();
}

float PhysicsObject::CalculateMass()
{
	return 0.0f;
}

float PhysicsObject::CalculateMomentOfInertia(Vec2 centreOfMass, std::vector<Vec2>& points, float pointWeight)
{
	return 0.0f;
}

void PhysicsObject::Rotate(float amnt)
{
}

void PhysicsObject::ApplyForce(Vec2 force)
{
	accumulatedLinearForce += force;
}

void PhysicsObject::ApplyImpulse(Vec2 force)
{
	linearVelocity += force * inverseMass;
}


