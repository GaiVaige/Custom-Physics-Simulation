#include "CollisionSolver.h"
#include "Collider.h"
#include "Circle.h"
#include "Polygon.h"


CollisionInfo GJKCollSolver::CheckForCollision(const Collider* colA, const Collider* colB)
{
    Simplex s;

    Vec2 support = Support(colA, colB, Vec2{ 1, 0 });
    s.pushfront(support);

    Vec2 searchDirection = -support;

    while (true) {
        support = Support(colA, colB, searchDirection);

        if (Dot(support, searchDirection) <= 0) return CollisionInfo(false);

        s.pushfront(support);

        if (NextSimplex(s, searchDirection)) {
            return CollisionInfo(true);
        }
    }
}
Vec2 GJKCollSolver::Support(const Collider* colA, const Collider* colB, Vec2 direction) const
{
    return colA->GetFurthestPoint(direction) - colB->GetFurthestPoint(direction);
}

bool GJKCollSolver::NextSimplex(Simplex& points, Vec2& direction)
{
    switch (points.size()) {
    case 2: return Line(points, direction);
    case 3: return Triangle(points, direction);
    }
    return false;
}

bool GJKCollSolver::Line(Simplex& points, Vec2& direction)
{
    Vec2 a = points[0];
    Vec2 b = points[1];

    Vec2 ab = b - a;
    Vec2 ao = -a;

    if (SameDirection(ab, ao)) {
        direction = ao;
    }
    else {
        points.flush();
        points.pushfront(a);
        direction = ao;
    }
    return false;
}

bool GJKCollSolver::Triangle(Simplex& points, Vec2& direction)
{

    Vec2 a = points[0];
    Vec2 b = points[1];
    Vec2 c = points[2];

    Vec2 ab = b - a;
    Vec2 ac = c - a;
    Vec2 ao = -a;

    Vec2 abc = ab.GetRotatedBy90();

    if (SameDirection(ab.GetRotatedBy90(), ao)) {
        if (SameDirection(ac, ao)) {
            points = { a, c };
            direction = ac.GetRotatedBy90();
        }
        else {
            return Line(points = {a, b}, direction);
        }
    }
    else {
        if (SameDirection(ab.GetRotatedBy90(), ao)) {
            return Line(points = { a, b }, direction);
        }
        else {
            if (SameDirection(abc, ao)) {
                direction = abc;
            }
            else {
                points = { a, c, b };
                direction = -abc;
            }
        }
    }

    return true;
}

CollisionInfo CollisionSolver::DetectCollision(Collider* colA, Collider* colB)
{
    return DispatchForCorrectFunction(colA, colB);
}

void CollisionSolver::ResolveCollision(CollisionInfo colInfo)
{
    if (!colInfo.collided) return;

    float totalInvMass = colInfo.colliderA->GetInvMass() + colInfo.colliderB->GetInvMass();
    Vec2 BOffset = colInfo.normal * colInfo.depth * colInfo.colliderB->GetInvMass() / totalInvMass;
    colInfo.colliderB->Move(BOffset);
    Vec2 AOffset = -(colInfo.normal * colInfo.depth * colInfo.colliderA->GetInvMass() / totalInvMass);
    colInfo.colliderA->Move(AOffset);
    return;
}

CollisionInfo CollisionSolver::DispatchForCorrectFunction(Collider* colA, Collider* colB)
{
    switch(colA->GetShape()){
        case COLLIDERSHAPE::CIRCLE:
            switch(colB->GetShape()){
                case COLLIDERSHAPE::CIRCLE:
                    return CircleToCircle(static_cast<CircleCollider*>(colA), static_cast<CircleCollider*>(colB));
                case COLLIDERSHAPE::PLANE:
                    return CircleToPlane(colA, colB);
                case COLLIDERSHAPE::POLYGON:
                    return CircleToPolygon(colA, colB);
            }
            break;
        case COLLIDERSHAPE::POLYGON:
            switch(colB->GetShape()){
                case COLLIDERSHAPE::CIRCLE:
                    return CircleToPolygon(colB, colA);
                case COLLIDERSHAPE::PLANE:
                    return PolygonToPlane(colA, colB);
                case COLLIDERSHAPE::POLYGON:
                    return PolygonToPolygon(static_cast<PolygonCollider*>(colA), static_cast<PolygonCollider*>(colB));
            }
            break;
        case COLLIDERSHAPE::PLANE:
            switch(colB->GetShape()){
                case COLLIDERSHAPE::CIRCLE:
                    return CircleToPlane(colB, colA);
                case COLLIDERSHAPE::PLANE:
                    return PlaneToPlane(colA, colB);
                case COLLIDERSHAPE::POLYGON:
                    return PolygonToPlane(colB, colA);
            }
            break;
    }
}

CollisionInfo CollisionSolver::CircleToCircle(CircleCollider *colA, CircleCollider*colB) const
{
    CollisionInfo colInf;
    float distance = (colB->GetPos() - colA->GetPos()).GetMagnitude();
    float sumOfRadii = (colA->GetRadius() + colB->GetRadius());
    if (distance > sumOfRadii) {
        return CollisionInfo(false);
    }
    Vec2 normal = (colB->GetPos() - colA->GetPos()).GetNormalised();
    float penAmnt = sumOfRadii - distance;
    colInf.collided = true;
    colInf.colliderA = colA;
    colInf.colliderB = colB;
    colInf.normal = normal;
    colInf.depth = penAmnt;
    return colInf;
}

CollisionInfo CollisionSolver::CircleToPolygon(const Collider* colA, const Collider* colB) const
{
    return CollisionInfo();
}

CollisionInfo CollisionSolver::CircleToPlane(const Collider* colA, const Collider* colB) const
{
    return CollisionInfo();
}

CollisionInfo CollisionSolver::PolygonToPolygon(PolygonCollider* colA, PolygonCollider* colB) const
{
    float overlap = FLT_MAX;
    Vec2 smallest;
    for (int i = 0; i < colA->faces; i++) {
        Vec2 ax = colA->GetAxis(i);
        Vec2 v = colA->GetVert(i);
        Projection p1 = ProjectOnAxis(ax, colA->GetVerts(), colA->faces);
        Projection p2 = ProjectOnAxis(ax, colB->GetVerts(), colB->faces);

        if (!p1.Overlaps(p2)) {
            return false;
        }
        else {
            float o = p1.GetOverlap(p2);
            if (o < overlap) {
                overlap = o;
                smallest = ax;
            }

        }
    }

    for (int i = 0; i < colB->faces; i++) {
        Vec2 ax = colB->GetAxis(i);

        Projection p1 = ProjectOnAxis(ax, colA->GetVerts(), colA->faces);
        Projection p2 = ProjectOnAxis(ax, colB->GetVerts(), colB->faces);

        if (!p1.Overlaps(p2)) {
            return false;
        }
        else {
            float o = p1.GetOverlap(p2);
            if (o < overlap) {
                overlap = o;
                smallest = ax;
            }

        }
    }
    CollisionInfo colInfo;
    colInfo.collided = true;
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.depth = overlap;
    colInfo.normal = smallest;
    return colInfo;
}

CollisionInfo CollisionSolver::PolygonToPlane(const Collider* colA, const Collider* colB) const
{
    return CollisionInfo();
}

CollisionInfo CollisionSolver::PlaneToPlane(const Collider* colA, const Collider* colB) const
{
    return CollisionInfo();
}

Projection CollisionSolver::ProjectOnAxis(Vec2 axis, Vec2* points, int faces) const
{
    float min = Dot(axis, points[0]);
    float max = min;

    for (int i = 1; i < faces; i++) {
        float proj = Dot(axis, points[i]);
        if (proj < min) min = proj;
        else if (proj > max) proj = max;
    }

    return Projection(min, max);
}

bool Projection::Overlaps(Projection& p2)
{
    if (max >= p2.min || p2.max >= min) {
        return true;
    }
    return false;
}

float Projection::GetOverlap(Projection& p2)
{

    if (max >= p2.min) {
        return max - p2.min;
    }

    if (p2.max >= min) {
        return p2.max - min;
    }

    return 0.0f;
}
