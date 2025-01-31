#pragma once
#include "vec2.h"

class Collider;

enum COLLIDERTYPE{
	STATIC,
	DYNAMIC,
	TRIGGER
};

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
	Vec2 normal;
	float depth;
	Collider* colliderA;
	Collider* colliderB;
};

class Collider {
public:
	Vec2 GetPos() const { return position; }
	Vec2& SetPos(Vec2& pos);
	virtual Vec2 GetFurthestPoint(Vec2& direction) const { return Vec2(); };
	COLLIDERSHAPE GetShape() const { return shape; }
	void SetParent(PhysicsObject* p){parent = p;};
	PhysicsObject* GetParent(){return parent;};

	void Move(Vec2& offset);
	
	void SetInvMass(float m) { inverseMass = m; } 
	float GetInvMass() { return inverseMass; }

protected:
	float inverseMass;
	COLLIDERSHAPE shape;
	Vec2 position;
	PhysicsObject* parent;
};