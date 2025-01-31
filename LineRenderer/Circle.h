#pragma once
#include "PhysicsObject.h"
#include <memory>
class LineRenderer;
class CircleCollider : public Collider {
public:
	CircleCollider(Vec2& pos, float& rad);
	~CircleCollider();
	CircleCollider(CircleCollider& c) = delete;
	CircleCollider& operator=(CircleCollider& c) = delete;
	Vec2 GetFurthestPoint(Vec2& direction) const override;

	float GetRadius() const { return radius; }
private:
	float radius;
};

class Circle : public PhysicsObject {
public:
	Circle(Vec2 pos, float radius);

	void Draw(LineRenderer* lines) override;
	float GetRadius() const {return radius;}

private:
	float radius;
};
