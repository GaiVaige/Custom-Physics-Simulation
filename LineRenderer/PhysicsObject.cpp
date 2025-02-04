#include "PhysicsObject.h"
#include <chrono>

PhysicsObject::PhysicsObject()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	GUID = nanos;
	velocity = Vec2(0, 0);
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
	Vec2 accel = ((accumulatedForce * inverseMass) - (velocity * dt)) - dragVec;
	position += velocity * dt;
	velocity += accel * dt;
	collider->SetPos(position);
	accumulatedForce = Vec2();
}

float PhysicsObject::CalculateMass()
{
	return 0.0f;
}

void PhysicsObject::ApplyForce(Vec2 force)
{
	accumulatedForce += force;
}

void PhysicsObject::ApplyImpulse(Vec2 force)
{
	velocity += force * inverseMass;
}


