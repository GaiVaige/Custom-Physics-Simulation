#include "CollisionSolver.h"
#include "Collider.h"
#include "Circle.h"
#include "Polygon.h"

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
                    return CircleToPolygon(static_cast<CircleCollider*>(colA), static_cast<PolygonCollider*>(colB));
            }
            break;
        case COLLIDERSHAPE::POLYGON:
            switch(colB->GetShape()){
                case COLLIDERSHAPE::CIRCLE:
                    return CircleToPolygon(static_cast<CircleCollider*>(colB), static_cast<PolygonCollider*>(colA));
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

CollisionInfo CollisionSolver::CircleToPolygon(CircleCollider* colA, PolygonCollider* colB) const
{
    float minDist = FLT_MAX;
    Vec2 closestDist;
    Vec2 axis;

    Vec2 offset = colB->GetPos() - colA->GetPos();
    Vec2 vertSave;
    for (Vec2 vert : colB->GetVerts()) {
        Vec2 delta = Vec2(vert.x - colA->GetPos().x, vert.y - colA->GetPos().y);
    
        float dist = delta.GetMagnitude();
    
        if (dist < minDist) {
            minDist = dist;
            closestDist = delta;
            vertSave = vert;
        }
    }
    
    axis = closestDist.GetNormalised();
    
    std::vector<Vec2> tempA = { vertSave };
    Projection proj = ProjectOnAxis(axis, tempA);
    //float sOff = Dot(axis, vertSave);
    //proj.min += sOff;
    //proj.max += sOff;


    std::vector<Vec2> temp = { colA->GetPos() };
    Projection circleProj = ProjectOnAxis(axis, temp);
    circleProj.min -= colA->GetRadius();
    circleProj.max += colA->GetRadius();
    
    if (!circleProj.Overlaps(proj)) {
        float overlap = FLT_MAX;
        Vec2 smallest;
        for (int i = 0; i < colB->GetVerts().size(); i++) {
            Vec2 ax = colB->GetAxis(i);

            Projection p1 = ProjectOnAxis(ax, temp);
            p1.min -= colA->GetRadius();
            p1.max += colA->GetRadius();
            Projection p2 = ProjectOnAxis(ax, colB->GetVerts());

            if (!p1.Overlaps(p2)) {
                return CollisionInfo(false);
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
    
    
    CollisionInfo colInfo;
    colInfo.collided = true;
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.depth = circleProj.GetOverlap(proj);
    colInfo.normal = axis;
    return colInfo;
}

CollisionInfo CollisionSolver::CircleToPlane(const Collider* colA, const Collider* colB) const
{
    return CollisionInfo();
}

CollisionInfo CollisionSolver::PolygonToPolygon(PolygonCollider* colA, PolygonCollider* colB) const
{
    float overlap = FLT_MAX;
    Vec2 smallest;
    for (int i = 0; i < colA->GetVerts().size(); i++) {
        Vec2 ax = colA->GetAxis(i);

        Projection p1 = ProjectOnAxis(ax, colA->GetVerts());
        Projection p2 = ProjectOnAxis(ax, colB->GetVerts());

        if (!p1.Overlaps(p2)) {
            return CollisionInfo(false);
        }
        else {
            float o = p1.GetOverlap(p2);
            if (o < overlap) {
                overlap = o;
                smallest = ax;
            }

        }
    }

    for (int i = 0; i < colB->GetVerts().size(); i++) {
        Vec2 ax = colB->GetAxis(i);

        Projection p1 = ProjectOnAxis(ax, colA->GetVerts());
        Projection p2 = ProjectOnAxis(ax, colB->GetVerts());

        if (!p1.Overlaps(p2)) {
            return CollisionInfo(false);
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

Projection CollisionSolver::ProjectOnAxis(Vec2 axis, std::vector<Vec2> points) const
{
    float min = Dot(axis, points[0]);
    float max = min;

    for (int i = 1; i < points.size(); i++) {
        float proj = Dot(axis, points[i]);
        if (proj < min) min = proj;
        if (proj > max) max = proj;
    }

    return Projection(min, max);
}

bool Projection::Overlaps(Projection& p2)
{
    float lsum = std::max(max, p2.max) - std::min(min, p2.min);
    float ssum = (max - min) + (p2.max - p2.min);
    return lsum <= ssum;
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
