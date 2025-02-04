#include "Circle.h"
#include "Maths.h"
#include "LineRenderer.h"
CircleCollider::CircleCollider(Vec2 pos, float rad)
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

void CircleCollider::DrawContactPoints(LineRenderer* lines)
{
    lines->SetColour(Colour::BLUE);
    for (Vec2 v : contactPoints) {
        lines->DrawCircle(v, .2);
    }
    lines->SetColour(Colour::WHITE);
}

Circle::Circle(Vec2 pos, float radius, float elas, PHYSICSTYPE t)
    : radius(radius)
{
    position = pos;
    elasticity = elas;
    collider = new CircleCollider(position, radius);
    collider->SetParent(this);
    type = t;
    if (type == STATIC) {
        collider->SetInvMass(0);
    }
    else {
        inverseMass = 1.0f / CalculateMass();
        collider->SetInvMass(inverseMass);
    }
}

void Circle::Draw(LineRenderer* lines) const
{
    lines->DrawCircle(position, radius);
    //lines->DrawLineWithArrow(position, position + velocity);
    //CircleCollider* p = static_cast<CircleCollider*>(collider);
    //p->DrawContactPoints(lines);
}

float Circle::CalculateMass()
{
    return PI * (radius * radius);
}
