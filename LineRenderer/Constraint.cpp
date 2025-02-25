#include "Constraint.h"
#include "PhysicsObject.h"
void PinConstraint::Constrain(PhysicsObject* object)
{
	object->SetPosition(position);
	object->SetVelocity(Vec2(0, 0));
}
