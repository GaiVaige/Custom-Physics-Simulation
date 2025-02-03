#include "CollisionSolver.h"
#include "Collider.h"
#include "Circle.h"
#include "Polygon.h"
#include "Plane.h"



Edge CollisionSolver::Best(std::vector<Vec2> verts, Vec2 axis) const {
    float max = -FLT_MAX;
    int furthestVertInd = -1;
    for (int i = 0; i < verts.size(); i++) {
        float proj = Dot(verts[i], axis);
        if (proj > max) {
            max = proj;
            furthestVertInd = i;
        }
    }

    Vec2 furthestVert = verts[furthestVertInd];
    Vec2 nextVert = verts[furthestVertInd == verts.size() - 1 ? 0 : furthestVertInd + 1];
    Vec2 previousVert = verts[furthestVertInd == 0 ? verts.size() - 1 : furthestVertInd - 1];

    Vec2 leftWard = (furthestVert - nextVert).GetNormalised();
    Vec2 rightWard = (furthestVert - previousVert).GetNormalised();

    if (Dot(rightWard, axis) <= Dot(leftWard, axis)) {
        return Edge(furthestVert, previousVert, furthestVert);
    }
    else {
        return Edge(furthestVert, furthestVert, nextVert);
    }

}

std::vector<Vec2> CollisionSolver::Clip(Vec2 incEdgeA, Vec2 incEdgeB, Vec2 refEdge, float amnt) const
{
    std::vector<Vec2> clippedPoints;

    float d1 = Dot(refEdge, incEdgeA) - amnt;
    float d2 = Dot(refEdge, incEdgeB) - amnt;

    if (d1 >= 0.0) clippedPoints.push_back(incEdgeA);
    if (d2 >= 0.0) clippedPoints.push_back(incEdgeB);

    if (d1 * d2 < 0) {
        Vec2 edge = incEdgeB - incEdgeA;
        float sc = d1 / (d1 - d2);
        edge *= sc;
        edge += incEdgeA;

        clippedPoints.push_back(edge);
    }

    return clippedPoints;
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
                    return CircleToPlane(static_cast<CircleCollider*>(colA), static_cast<PlaneCollider*>(colB));
                case COLLIDERSHAPE::POLYGON:
                    return CircleToPolygon(static_cast<CircleCollider*>(colA), static_cast<PolygonCollider*>(colB));
            }
            break;
        case COLLIDERSHAPE::POLYGON:
            switch(colB->GetShape()){
                case COLLIDERSHAPE::CIRCLE:
                    return CircleToPolygon(static_cast<CircleCollider*>(colB), static_cast<PolygonCollider*>(colA));
                case COLLIDERSHAPE::PLANE:
                    return PolygonToPlane(static_cast<PolygonCollider*>(colA), static_cast<PlaneCollider*>(colB));
                case COLLIDERSHAPE::POLYGON:
                    return PolygonToPolygon(static_cast<PolygonCollider*>(colA), static_cast<PolygonCollider*>(colB));
            }
            break;
        case COLLIDERSHAPE::PLANE:
            switch(colB->GetShape()){
                case COLLIDERSHAPE::CIRCLE:
                    return CircleToPlane(static_cast<CircleCollider*>(colB), static_cast<PlaneCollider*>(colA));
                case COLLIDERSHAPE::PLANE:
                    return CollisionInfo();
                case COLLIDERSHAPE::POLYGON:
                    return PolygonToPlane(static_cast<PolygonCollider*>(colB), static_cast<PlaneCollider*>(colA));
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

    Vec2 vertSave;
    for (Vec2 vert : colB->GetVerts()) {
        Vec2 delta = Vec2(vert.x - colA->GetPos().x, vert.y - colA->GetPos().y);
    
        float dist = delta.GetMagnitude();
    
        if (dist < minDist) {
            minDist = dist;
            closestDist = delta;
        }
    }
    
    axis = closestDist.GetNormalised();
    float overlap = FLT_MAX;
    Vec2 smallest;

    std::vector<Vec2> tempC = { colA->GetPos() };
    Projection p = ProjectOnAxis(axis, tempC);
    p.min -= colA->GetRadius();
    p.max += colA->GetRadius();


    for (int i = 0; i < colB->GetVerts().size(); i++) {

        Projection p2 = ProjectOnAxis(axis, colB->GetVerts());

        if (!p.Overlaps(p2)) {
            return CollisionInfo(false);
        }
        else {
            float o = p.GetOverlap(p2);
            if (o < overlap) {
                overlap = o;
                smallest = axis;
            }

        }
    }

    for (int i = 0; i < colB->GetVerts().size(); i++) {
        Vec2 ax = colB->GetAxis(i);

        Projection p1 = ProjectOnAxis(ax, tempC);
        p1.min -= colA->GetRadius();
        p1.max += colA->GetRadius();
        Projection p2 = ProjectOnAxis(ax, colB->GetVerts());

        if (!p1.Overlaps(p2)) {
            return CollisionInfo(false);
        }
        else {
            float o = p2.GetOverlap(p1);
            if (o < overlap) {
                overlap = o;
                smallest = -ax;
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

CollisionInfo CollisionSolver::CircleToPlane(CircleCollider* colA, PlaneCollider* colB) const
{
    float distance = Dot(colA->GetPos(), colB->GetAxis()) + colB->GetDisplacement();

    CollisionInfo colInfo;
    colInfo.collided = distance < colA->GetRadius();
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.normal = colB->GetAxis();
    colInfo.depth = distance - colA->GetRadius();

    return colInfo;
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
            float o = p2.GetOverlap(p1);
            if (o < overlap) {
                overlap = o;
                smallest = -ax;
            }

        }

    }

    /*Edge e1 = Best(colA->GetVerts(), smallest);

    Edge e2 = Best(colB->GetVerts(), -smallest);

    Edge ref, inc;

    bool flip = false;

    if (abs(Dot(e1.v1 - e1.v2, smallest) <= abs(Dot(e2.v1 - e2.v2, smallest)))) {
        ref = e1;
        inc = e2;
    }
    else {
        ref = e2;
        inc = e1;
        flip = true;
    }

    Vec2 refV = ref.v2 - ref.v1;
    refV.Normalise();

    float overlapA = Dot(refV, ref.v1);
    std::vector<Vec2> cpA = Clip(inc.v1, inc.v2, refV, overlapA);

    if (cpA.size() < 2) return CollisionInfo(false);

    float overlapB = Dot(refV, ref.v2);
    cpA = Clip(cpA[0], cpA[1], -refV, -overlapB);

    if (cpA.size() < 2) return CollisionInfo(false);

    Vec2 refNormal = Vec2(-refV.y, refV.x);

    if (flip) refNormal *= -1;

    float max = Dot(refNormal, ref.max);

    if (Dot(refNormal, cpA[0]) - max < 0.0){
        cpA.erase(cpA.begin());
        if (Dot(refNormal, cpA[0]) - max < 0.0) {
            cpA.erase(cpA.begin());
        }
    }
    else if (Dot(refNormal, cpA[1]) - max < 0.0) {
        cpA.erase(cpA.begin() + 1);
    }
    for (Vec2 v : cpA) {
        colA->contactPoints.push_back(v);
        colB->contactPoints.push_back(v);
    }*/


    CollisionInfo colInfo;
    colInfo.collided = true;
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.depth = overlap;
    colInfo.normal = smallest;
    return colInfo;
}

CollisionInfo CollisionSolver::PolygonToPlane(PolygonCollider* colA, PlaneCollider* colB) const
{
    float overlap = FLT_MAX;
    for (int i = 0; i < colA->GetVerts().size(); i++) {
        float dist = Dot(colA->GetVert(i), colB->GetAxis()) + colB->GetDisplacement();
        if (dist < overlap) {
            overlap = dist;
        }
    }
    CollisionInfo colInfo;
    colInfo.collided = (overlap <= 0);
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.normal = colB->GetAxis();
    colInfo.depth = overlap;
    return colInfo;
}

Projection CollisionSolver::ProjectOnAxis(Vec2 axis, std::vector<Vec2> points) const
{
    float min = FLT_MAX;
    float max = -FLT_MAX;

    for (int i = 0; i < points.size(); i++) {
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
