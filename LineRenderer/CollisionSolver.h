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

class CollisionSolver {
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

