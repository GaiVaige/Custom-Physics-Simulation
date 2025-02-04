#pragma once
#include "PhysicsObject.h"
class LineRenderer;
class CircleCollider : public Collider {
public:
	CircleCollider(Vec2& pos, float& rad);
	~CircleCollider();
	CircleCollider(CircleCollider& c) = delete;
	CircleCollider& operator=(CircleCollider& c) = delete;
	Vec2 GetFurthestPoint(Vec2& direction) const override;
	void DrawContactPoints(LineRenderer* lines);
	float GetRadius() const { return radius; }
private:
	float radius;
};

class Circle : public PhysicsObject {
public:
	Circle(Vec2 pos, float radius, PHYSICSTYPE t = PHYSICSTYPE::DYNAMIC);

	void Draw(LineRenderer* lines) const override;
	float GetRadius() const {return radius;}
	float CalculateMass() override;

private:
	float radius;
};
