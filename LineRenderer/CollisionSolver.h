#pragma once
#include "Vec2.h"
#include <vector>
#include "Collider.h"
class CircleCollider;
class PolygonCollider;
//checking structure for the overlap



struct Projection {
	Projection(float mi, float ma) : min(mi), max(ma) {};
	float min;
	float max;

	bool Overlaps(Projection& p2);
	float GetOverlap(Projection& p2);
};

class CollisionSolver{
public:
	CollisionInfo DetectCollision(Collider* colA, Collider* colB);
	void ResolveCollision(CollisionInfo colInfo);
private:
	CollisionInfo DispatchForCorrectFunction(Collider* colA, Collider* colB);

	CollisionInfo CircleToCircle(CircleCollider* colA, CircleCollider* colB) const;
	CollisionInfo CircleToPolygon(CircleCollider* colA, PolygonCollider* colB) const;
	CollisionInfo CircleToPlane(const Collider* colA, const Collider* colB) const;
	CollisionInfo PolygonToPolygon(PolygonCollider* colA, PolygonCollider* colB) const;
	CollisionInfo PolygonToPlane(const Collider* colA, const Collider* colB) const;
	CollisionInfo PlaneToPlane(const Collider* colA, const Collider* colB) const;


	Projection ProjectOnAxis(Vec2 axis, std::vector<Vec2> points) const;

};