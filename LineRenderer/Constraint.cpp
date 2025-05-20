#include "Constraint.h"
#include "PhysicsObject.h"
void PinConstraint::Constrain(PhysicsObject* object, float dt)
{
	object->SetVelocity(Vec2(0, 0));
	object->accumulatedLinearForce = Vec2();
	object->constraintResolvedPosition = true;
}

void SpringConstraint::Constrain(PhysicsObject* object, float dt)
{
	float dist = (position - object->GetPos()).GetMagnitude();
	if (dist < .05) {
		return;
	}
	object->ApplyImpulse((position - object->GetPos()).GetNormalised() * springStrength * dist);
	object->constraintResolvedPosition = true;
}

void HingeJoint::Constrain(PhysicsObject* object, float dt)
{
	float rotAccel = (object->accumulatedAngularForce * object->inverseMomentOfInertia);
	object->RotateAbout(object->angularVelocity * dt, this->position);
	object->angularVelocity += rotAccel * dt;
	object->angularVelocity -= object->angularVelocity * object->angularDrag * dt;
	object->accumulatedAngularForce = 0;
	PinConstraint::Constrain(object, dt);
	object->constraintResolvedRotation = true;
}

void Joint::Attach(PhysicsObject* object)
{
	object->constraint = this;
}
