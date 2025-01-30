#include "Circle.h"
#include "Maths.h"
#include "LineRenderer.h"
CircleCollider::CircleCollider(Vec2& pos, float& rad)
{
    position = pos;
    radius = rad;
}
CircleCollider::~CircleCollider()
{
}

Vec2 CircleCollider::GetFurthestPoint(Vec2& direction) const
{
    Vec2 nDir = direction.GetNormalised();

    return position + (nDir * radius);
}

void Circle::Draw(LineRenderer* lines)
{
    lines->DrawCircle(position, radius);
}
