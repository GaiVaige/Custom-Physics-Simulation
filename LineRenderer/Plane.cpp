#include "Plane.h"
#include "LineRenderer.h"
Plane::Plane(Vec2 normal, float displacement, float elas)
{
	direction = normal.GetNormalised();
	position = -direction * displacement;
	elasticity = elas;
	collider = new PlaneCollider(direction, displacement);
	type = STATIC;
	collider->SetParent(this);

	inverseMomentOfInertia = 0;
	centreOfMassDisplacement = Vec2(0, 0);

}

void Plane::Draw(LineRenderer* lines) const
{
	Vec2 start = direction.GetRotatedBy270();
	Vec2 end = direction.GetRotatedBy90();
	lines->SetColour(Colour::RED);
	lines->AddPointToLine(position + start * 1000);
	lines->AddPointToLine(position + end * 1000);
	lines->FinishLineStrip();
	//lines->SetColour(Colour::BLUE);
	//lines->DrawLineWithArrow(position, position + direction);
	lines->SetColour(Colour::WHITE);
}

PlaneCollider::PlaneCollider(Vec2 normal, float disp)
{
	direction = normal;
	displacement = disp;
	position = direction * displacement;
	shape = PLANE;
}
