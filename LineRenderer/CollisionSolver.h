#pragma once
#include "Vec2.h"
#include <vector>
#include "Collider.h"
class CircleCollider;
class PolygonCollider;
class PlaneCollider;

//checking structure for the overlap

struct Edge {
	Edge() {};
	Edge(Vec2 max, Vec2 v1, Vec2 v2) : max(max), v1(v1), v2(v2) {};
	Vec2 max, v1, v2;
};

struct Projection {
	Projection() {};
	Projection(float mi, float ma) : min(mi), max(ma) {};
	float min;
	float max;

	bool Overlaps(Projection& p2);
	float GetOverlap(Projection& p2);
};

class CollisionSolver{
public:
	CollisionInfo DetectCollision(Collider* colA, Collider* colB);
	void ResolveCollision(CollisionInfo colInfo, float dt);
private:
	CollisionInfo DispatchForCorrectFunction(Collider* colA, Collider* colB);

	CollisionInfo CircleToCircle(CircleCollider* colA, CircleCollider* colB) const;
	CollisionInfo CircleToPolygon(CircleCollider* colA, PolygonCollider* colB) const;
	CollisionInfo CircleToPlane(CircleCollider* colA, PlaneCollider* colB) const;
	CollisionInfo PolygonToPolygon(PolygonCollider* colA, PolygonCollider* colB) const;
	CollisionInfo PolygonToPlane(PolygonCollider* colA, PlaneCollider* colB) const;


	Projection ProjectOnAxis(Vec2 axis, std::vector<Vec2> points) const;

	Edge Best(std::vector<Vec2> verts, Vec2 axis) const;
	std::vector<Vec2> Clip(Vec2 incEdgeA, Vec2 incEdgeB, Vec2 refEdge, float amnt) const;
};