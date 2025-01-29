#include "CollisionSolver.h"
#include "Collider.h"
CollisionInfo CollisionSolver::CheckForCollision(const Collider& colA, const Collider& colB)
{
    Simplex s;

    Vec2 support = Support(colA, colB, Vec2{ 1, 0 });
    s.pushfront(support);

    Vec2 searchDirection = -support;

    while (true) {
        support = Support(colA, colB, searchDirection);

        if (Dot(support, searchDirection) <= 0) return CollisionInfo(false);

        s.pushfront(support);

    }
}
Vec2 CollisionSolver::Support(const Collider& colA, const Collider& colB, Vec2 direction) const
{
    return colA.GetFurthestPoint(direction) - colB.GetFurthestPoint(direction);
}

bool CollisionSolver::NextSimplex(Simplex& points, Vec2& direction)
{
    switch (points.size()) {
    case 2: return Line(points, direction);
    case 3: return Triangle(points, direction);
    }
    return false;
}

bool CollisionSolver::Line(Simplex& points, Vec2& direction)
{
    Vec2 a = points[0];
    Vec2 b = points[1];

    Vec2 ab = b - a;
    Vec2 ao = -a;

    if (SameDirection(ab, ao)) {
        direction;
    }
    else {
        points.flush();
        points.pushfront(a);
        direction = ao;
    }

    return false;
}

bool CollisionSolver::Triangle(Simplex& points, Vec2& direction)
{
    return false;
}
