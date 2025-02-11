#include "PhysicsObject.h"
#include <chrono>
#include "LineRenderer.h"
#include "math.h"
PhysicsObject::PhysicsObject()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	GUID = nanos;
	linearVelocity = Vec2(0, 0);
	up = Vec2(0, 1);
	right = Vec2(1, 0);
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
	//position
	Vec2 accel = (accumulatedLinearForce * inverseMass);
	position += linearVelocity * dt;
	linearVelocity += accel * dt;
	linearVelocity -= linearVelocity * linearDrag * dt;
  collider->SetPos(position);
	accumulatedLinearForce = Vec2();

	//rotation
	float rotAccel = (accumulatedAngularForce/momentOfIntertia);
	Rotate(angularVelocity * dt);
	angularVelocity += rotAccel * dt;
	angularVelocity -= angularVelocity * angularDrag * dt;
	accumulatedAngularForce = 0;
}

float PhysicsObject::CalculateMass()
{
	return 0.0f;
}

float PhysicsObject::CalculateMomentOfInertia(Vec2 centreOfMass, std::vector<Vec2>& points, float pointWeight)
{
	float totalMOI = 0;
	for (Vec2 p : points) {
		totalMOI += (centreOfMass - (p + GetPos())).GetMagnitudeSquared() * pointWeight;
	}
	return totalMOI;
}

void PhysicsObject::DrawOrientingAxes(LineRenderer* lines) const
{
	lines->DrawLineSegment(position, position + up * 2, Colour::GREEN);
	lines->DrawLineSegment(position, position + right * 2, Colour::RED);
}

void PhysicsObject::Rotate(float amnt)
{
	while (orientation > DegToRad(360)) {
		orientation -= DegToRad(360);
	}
}

void PhysicsObject::ApplyForce(Vec2 force)
{
	accumulatedLinearForce += force;
}

void PhysicsObject::ApplyImpulse(Vec2 force)
{
	linearVelocity += force * inverseMass;
}

void PhysicsObject::ApplyAngularForce(Vec2 force, Vec2 pos)
{
	accumulatedAngularForce += (force.y * pos.x - force.x * pos.y)/momentOfIntertia;
}

void PhysicsObject::ApplyAngularImpulse(Vec2 force, Vec2 pos)
{
	float f = (force.y * pos.x - force.x * pos.y) / momentOfIntertia;
	angularVelocity += f;
}

Vec2 PhysicsObject::GetVelocityAt(Vec2 pos) const
{
	return linearVelocity + (pos - position).GetRotatedBy90() * angularVelocity;
}


