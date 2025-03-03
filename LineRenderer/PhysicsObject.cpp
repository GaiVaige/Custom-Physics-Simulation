#include "PhysicsObject.h"
#include <chrono>
#include "LineRenderer.h"
#include "math.h"
#include "Constraint.h"
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
	if (constraint != nullptr) {
		delete constraint;
	}
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
	if (constraint != nullptr) {
		constraint->Constrain(this, dt);
	}
	Vec2 accel = (GRAVITY * (int)useGravity) + accumulatedLinearForce * inverseMass;

	if (!constraintResolvedPosition) {
		position += linearVelocity * dt;
		linearVelocity += accel * dt;
		linearVelocity -= linearVelocity * linearDrag * dt;
		accumulatedLinearForce = Vec2();
	}

	if (!constraintResolvedRotation) {
		//rotation
		float rotAccel = (accumulatedAngularForce * inverseMomentOfInertia);
		Rotate(angularVelocity * dt);
		angularVelocity += rotAccel * dt;
		angularVelocity -= angularVelocity * angularDrag * dt;
		accumulatedAngularForce = 0;
	}
	collider->SetPos(position);

	constraintResolvedPosition = false;
	constraintResolvedRotation = false;

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

void PhysicsObject::Unload()
{
}

void PhysicsObject::Rotate(float amnt)
{
	while (orientation > DegToRad(360)) {
		orientation -= DegToRad(360);
	}
}

void PhysicsObject::RotateAbout(float amnt, Vec2 pos)
{
	Vec2 tP = GetPos();
	tP -= pos;
	tP.RotateBy(amnt);
	tP += pos;
	position = tP;
	Rotate(amnt);
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
	accumulatedAngularForce += (force.y * pos.x - force.x * pos.y) * inverseMomentOfInertia;
}

void PhysicsObject::ApplyImpulseAt(Vec2 force, Vec2 pos)
{
	//float torque = Dot(pos, force);
	float f = (force.y * pos.x - force.x * pos.y);
	angularVelocity += f * inverseMomentOfInertia;
	linearVelocity += (force * inverseMass);
}

Vec2 PhysicsObject::GetVelocityAt(Vec2 pos) const
{
	return linearVelocity + (pos - position).GetRotatedBy90() * angularVelocity;
}


