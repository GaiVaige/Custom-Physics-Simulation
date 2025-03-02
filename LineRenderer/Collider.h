#pragma once
#include "vec2.h"
#include <vector>
class Collider;


enum COLLIDERSHAPE{
	CIRCLE,
	POLYGON,
	PLANE
};

class PhysicsObject;
struct CollisionInfo {
	CollisionInfo() {};
	CollisionInfo(bool collided) : collided(collided) {};
	CollisionInfo(bool collided, Vec2 norm, float depth) : collided(collided), normal(norm), depth(depth) {};
	bool collided;
	float depth;
	Vec2 normal;
	Vec2 contactPoint;
	Collider* colliderA;
	Collider* colliderB;
};

class Collider {
public:
	Vec2 GetPos() const { return position; }
	virtual void SetPos(Vec2& pos);
	COLLIDERSHAPE GetShape() const { return shape; }
	void SetParent(PhysicsObject* p){parent = p;};
	PhysicsObject* GetParent(){return parent;};

	void Move(Vec2& offset);
	virtual void Rotate(float amnt) = 0;


protected:
	COLLIDERSHAPE shape;
	Vec2 position;
	PhysicsObject* parent;

};