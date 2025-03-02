#pragma once
#include "PhysicsObject.h"
class LineRenderer;
class CircleCollider : public Collider {
public:
	CircleCollider(Vec2 pos, float rad);
	CircleCollider(CircleCollider& c) = delete;
	CircleCollider& operator=(CircleCollider& c) = delete;
	float GetRadius() const { return radius; }
	void Rotate(float amnt) {};
private:
	float radius;
};

class Circle : public PhysicsObject {
public:
	Circle(Vec2 pos, float radius, float elas, PHYSICSTYPE t = PHYSICSTYPE::DYNAMIC);

	void Draw(LineRenderer* lines) const override;
	void Rotate(float amnt) override;
	float GetRadius() const {return radius;}
	float CalculateMass() override;

private:
	float radius;
};
