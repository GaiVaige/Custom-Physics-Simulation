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
        lines->DrawCircle(v + GetPos(), .2);
    }
    lines->SetColour(Colour::WHITE);
}

void CircleCollider::Rotate(float amnt)
{
}

Circle::Circle(Vec2 pos, float radius, float elas, PHYSICSTYPE t)
    : radius(radius)
{
    position = pos;
    centreOfMassDisplacement = Vec2(0,0);
    elasticity = elas;
    collider = new CircleCollider(position, radius);
    collider->SetParent(this);
    type = t;
    float m = CalculateMass();
    inverseMass = 1.0f / m;
    float moi = m * (radius * radius) * .4;
    SetMOI(moi);
    collider->SetInvMass(inverseMass);
    if (type == STATIC) {
        collider->SetInvMass(0);
        inverseMass = 0;
        momentOfIntertia = FLT_MAX;
    }

}

void Circle::Draw(LineRenderer* lines) const
{
    lines->DrawCircle(position, radius);
    //lines->DrawLineWithArrow(position, position + velocity);
    CircleCollider* p = static_cast<CircleCollider*>(collider);
    p->DrawContactPoints(lines);
    DrawOrientingAxes(lines);
    lines->DrawLineWithArrow(position, position + GetVelocity());
}

void Circle::Rotate(float amnt)
{
    up.RotateBy(amnt);
    right.RotateBy(amnt);
    orientation += amnt;
    PhysicsObject::Rotate(amnt);
}

float Circle::CalculateMass()
{
    float mass = PI * (radius * radius);
    return mass/3;
}
