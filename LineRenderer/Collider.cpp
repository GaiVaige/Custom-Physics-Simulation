#include "Collider.h"
#include "PhysicsObject.h"
void Collider::SetPos(Vec2& pos)
{
    position = pos;
}

void Collider::Move(Vec2& offset)
{
    parent->OffsetPosition(offset);
}

//void Collider::Rotate(float amnt)
//{
//}



