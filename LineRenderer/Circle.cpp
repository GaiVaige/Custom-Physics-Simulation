#include "Circle.h"
#include "Maths.h"
#include "LineRenderer.h"
CircleCollider::CircleCollider(Vec2& pos, float& rad)
{
    position = pos;
    radius = rad;
    shape = CIRCLE;
}
CircleCollider::~CircleCollider()
{
}

Vec2 CircleCollider::GetFurthestPoint(Vec2& direction) const
{
    Vec2 nDir = direction.GetNormalised();

    return position + (nDir * radius);
}

Circle::Circle(Vec2 pos, float radius)
    : radius(radius)
{
    position = pos;
    collider = new CircleCollider(position, radius);
    collider->SetInvMass(1.0f/(radius * radius));
    collider->SetParent(this);
}

void Circle::Draw(LineRenderer* lines)
{
    lines->DrawCircle(position, radius);
}
