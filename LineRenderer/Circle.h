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

	float GetRadius() { return radius; }
private:
	float radius;
};

class Circle : public PhysicsObject {
public:
	Circle(Vec2 pos, float radius) :radius(radius) { position = pos; collider = new CircleCollider(position, radius); };

	CircleCollider* collider;

	void Draw(LineRenderer* lines) override;

private:
	float radius;
};
