#include "PhysicsObject.h"
#include <chrono>

PhysicsObject::PhysicsObject()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	GUID = nanos;
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
	position += (velocity * dt) * collider->GetInvMass();
	collider->SetPos(position);
	velocity -= (velocity * dt * .1f);
}

float PhysicsObject::CalculateMass()
{
	return 0.0f;
}

void PhysicsObject::ApplyForce(Vec2 force)
{
	//velocity accumulator
}

void PhysicsObject::ApplyImpulse(Vec2 force)
{
	velocity += force * mass;
}


