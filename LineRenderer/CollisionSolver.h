#pragma once
#include "Vec2.h"
#include <array>
#include "Collider.h"

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

	void pushfront(Vec2& v) {
		points = { v, points[0], points[1] };
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

class CollisionSolver {
public:
	CollisionInfo CheckForCollision(const Collider& colA, const Collider& colB);

	Vec2 Support(const Collider& colA, const Collider& colB, Vec2 direction) const;

	bool NextSimplex(Simplex& points, Vec2& direction);

private:

	//helper functions
	bool SameDirection(Vec2& a, Vec2& b) { return Dot(a, b) > 0; }
	bool Line(Simplex& points, Vec2& direction);
	bool Triangle(Simplex& points, Vec2& direction);
};

