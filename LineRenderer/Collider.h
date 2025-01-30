#pragma once
#include "vec2.h"

class Collider;
struct CollisionInfo {
	CollisionInfo() {};
	CollisionInfo(bool collided) : collided(collided) {};
	bool collided;
	Vec2 normal, position, depth;
};

class Collider {
public:
	Vec2 GetPos() { return position; }
	Vec2& SetPos(Vec2& pos);

	virtual Vec2 GetFurthestPoint(Vec2& direction) const { return Vec2(); };


protected:
	Vec2 position;
};