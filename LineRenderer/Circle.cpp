#include "Circle.h"
#include "Maths.h"
CollisionInfo CircleCollider::IsOverlapping(Collider& other)
{
    CollisionInfo colDat;

    

    return colDat;
}

Vec2 CircleCollider::GetFurthestPoint(Vec2& direction) const
{
    Vec2 nDir = direction.GetNormalised();

    return position + (nDir * radius);
}
