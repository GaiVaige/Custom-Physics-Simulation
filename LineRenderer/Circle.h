#pragma once
#include "PhysicsObject.h"


class CircleCollider : public Collider {
public:
	CircleCollider(Vec2& pos, float& rad);
	~CircleCollider();
	CircleCollider(CircleCollider& c) = delete;
	CircleCollider& operator=(CircleCollider& c) = delete;

	CollisionInfo IsOverlapping(Collider& other) override;
	Vec2 GetFurthestPoint(Vec2& direction) const override;

	float GetRadius() { return radius; }
private:
	float radius;
};

class Circle : public PhysicsObject {

};
