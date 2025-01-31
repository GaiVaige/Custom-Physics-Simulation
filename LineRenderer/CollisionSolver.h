#pragma once
#include "Vec2.h"
#include <array>
#include "Collider.h"
class CircleCollider;
class PolygonCollider;
//checking structure for the overlap
struct Simplex {
private:
	std::array<Vec2, 3> points;
	int arrsize;

public:
	Simplex() {
		arrsize = 0;
	}


	int size() { return arrsize; }

	Simplex& operator=(std::initializer_list<Vec2> list) {
		arrsize = 0;

		for (Vec2 p : list) {
			points[arrsize] = p;
			arrsize++;
		}
		return *this;
	}


	void pushfront(Vec2& v) {
		points = { v, points[0], points[1] };
		arrsize++;
	}
	Vec2& operator[](int i) { return points[i]; }
	Vec2& begin() { return points[0]; }
	Vec2& end() {
		if (arrsize == 0) {
			return begin();
		}
		else {
			return points[arrsize - 1];
		}
	}
	void flush() { points = {}; }
};

class GJKCollSolver {
public:


	CollisionInfo CheckForCollision(const Collider* colA, const Collider* colB);

	Vec2 Support(const Collider* colA, const Collider* colB, Vec2 direction) const;

	bool NextSimplex(Simplex& points, Vec2& direction);


private:

	//helper functions
	bool SameDirection(const Vec2& a, const Vec2& b) const { return Dot(a, b) > 0; }
	bool Line(Simplex& points, Vec2& direction);
	bool Triangle(Simplex& points, Vec2& direction);
};

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
	CollisionInfo CircleToPolygon(const Collider* colA, const Collider* colB) const;
	CollisionInfo CircleToPlane(const Collider* colA, const Collider* colB) const;
	CollisionInfo PolygonToPolygon(PolygonCollider* colA, PolygonCollider* colB) const;
	CollisionInfo PolygonToPlane(const Collider* colA, const Collider* colB) const;
	CollisionInfo PlaneToPlane(const Collider* colA, const Collider* colB) const;


	Projection ProjectOnAxis(Vec2 axis, Vec2* points, int faces) const;

};