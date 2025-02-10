#include "Collider.h"
#include "PhysicsObject.h"
Vec2& Collider::SetPos(Vec2& pos)
{
    position = pos;
    return position;
}

void Collider::Move(Vec2& offset)
{
    parent->OffsetPosition(offset);
}

void Collider::Rotate(float amnt)
{
}

void Collider::EndTick()
{
    contactPoints.clear();
}


