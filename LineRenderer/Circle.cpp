#include "Circle.h"
#include "Maths.h"
#include "LineRenderer.h"
CircleCollider::CircleCollider(Vec2 pos, float rad)
{
    position = pos;
    radius = rad;
    shape = CIRCLE;
    

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
    inverseMomentOfInertia = (1.0f/moi);
    collider->SetInvMass(inverseMass);
    if (type == STATIC) {
        collider->SetInvMass(0);
        inverseMass = 0;
        inverseMomentOfInertia = 0;
    }

}

void Circle::Draw(LineRenderer* lines) const
{
    lines->DrawCircle(position, radius);
    CircleCollider* p = static_cast<CircleCollider*>(collider);
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
