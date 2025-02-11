#include "CollisionSolver.h"
#include "Collider.h"
#include "Circle.h"
#include "Polygon.h"
#include "Plane.h"

#include <iostream>

bool IsInside(Vec2 p, std::pair<Vec2, Vec2> edge) {

    Vec2 check = edge.first - edge.second;

    float f = PseudoCross(check, p) - PseudoCross(edge.first, edge.second);


    return f <= 0.0f;
}

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
    if (totalInvMass == 0) {
        return;
    }
    Vec2 velA = colInfo.colliderA->GetParent()->GetVelocity();
    Vec2 velB = colInfo.colliderB->GetParent()->GetVelocity();
    float elas = (colInfo.colliderA->GetParent()->elasticity + colInfo.colliderB->GetParent()->elasticity) / 2.f;


    Vec2 aAv = Vec2(0, 0);
    if (colInfo.colliderA->contactPoints.size() > 0) {
        for (Vec2 v : colInfo.colliderA->contactPoints) {
            aAv += v;
        }
        aAv /= colInfo.colliderA->contactPoints.size();
        aAv += colInfo.colliderA->GetPos();
    }
    float armA = aAv.GetMagnitudeSquared();
    float effmassA = 1 / 
        (colInfo.colliderA->GetInvMass() + (armA / colInfo.colliderA->GetParent()->momentOfIntertia));

    Vec2 bAv = Vec2(0, 0);
    if (colInfo.colliderB->contactPoints.size() > 0) {

        for (Vec2 v : colInfo.colliderB->contactPoints) {
            bAv += v;
        }
        bAv /= colInfo.colliderB->contactPoints.size();
        bAv += colInfo.colliderB->GetPos();
    }
    float armB = bAv.GetMagnitudeSquared();
    float effmassB = 1 /
        (colInfo.colliderB->GetInvMass() + (armB / colInfo.colliderB->GetParent()->momentOfIntertia));

    float j = -(1 + elas) * Dot((velB - velA), colInfo.normal) /
        Dot(colInfo.normal, colInfo.normal) * (1/effmassA + 1/effmassB);

    Vec2 relVelA = (colInfo.colliderA->GetParent()->GetVelocityAt(aAv + colInfo.colliderA->GetPos()));

    Vec2 relVelB = (colInfo.colliderB->GetParent()->GetVelocityAt(bAv + colInfo.colliderB->GetPos()));

    Vec2 totalRelVel = relVelA + relVelB;

    Vec2 BOffset = colInfo.normal * colInfo.depth * (colInfo.colliderB->GetInvMass() / totalInvMass);
    colInfo.colliderB->Move(BOffset);
    colInfo.colliderB->GetParent()->ApplyImpulse(colInfo.normal * colInfo.colliderB->GetInvMass() * j);
    colInfo.colliderB->GetParent()->ApplyAngularImpulse(colInfo.normal * totalRelVel.GetMagnitude(), bAv);



    Vec2 AOffset = (-colInfo.normal * colInfo.depth * (colInfo.colliderA->GetInvMass() / totalInvMass));
    colInfo.colliderA->Move(AOffset);
    colInfo.colliderA->GetParent()->ApplyImpulse(-colInfo.normal * colInfo.colliderA->GetInvMass() * j);
    colInfo.colliderA->GetParent()->ApplyAngularImpulse(-colInfo.normal * -totalRelVel.GetMagnitude(), aAv);

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
                    return CollisionInfo(false);
                case COLLIDERSHAPE::POLYGON:
                    return PolygonToPlane(static_cast<PolygonCollider*>(colB), static_cast<PlaneCollider*>(colA));
            }
            break;
    }
    return CollisionInfo(false);
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
    colA->contactPoints.push_back(normal * colA->GetRadius());
    colB->contactPoints.push_back(-normal * colB->GetRadius());
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
        Vec2 delta = Vec2(vert.x + colB->GetPos().x - colA->GetPos().x, vert.y + colB->GetPos().y - colA->GetPos().y);
    
        float dist = delta.GetMagnitude();
    
        if (dist < minDist) {
            minDist = dist;
            closestDist = delta;
        }
        else if (dist == minDist) {
            closestDist = (closestDist + colA->GetPos() + delta + colA->GetPos())/2;
            closestDist -= colA->GetPos();
            minDist = closestDist.GetMagnitude();
        }
    }
    
    axis = closestDist.GetNormalised();
    float overlap = FLT_MAX;
    Vec2 smallest;

    std::vector<Vec2> tempC = { colA->GetPos() };
    Projection p = ProjectOnAxis(axis, tempC);
    p.min -= colA->GetRadius();
    p.max += colA->GetRadius();

    std::vector<Vec2> vecBs = colB->GetVerts();
    for (Vec2& v : vecBs) {
        v.RotateBy(colB->GetParent()->GetOrientation());
        v += colB->GetPos();
        std::
        
    }

    Projection p2 = ProjectOnAxis(axis, vecBs);

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

    for (int i = 0; i < colB->GetVerts().size(); i++) {
        Vec2 ax = colB->GetAxis(i);

        Projection p1 = ProjectOnAxis(ax, tempC);
        p1.min -= colA->GetRadius();
        p1.max += colA->GetRadius();
        Projection p2 = ProjectOnAxis(ax, vecBs);

        if (!p2.Overlaps(p1)) {
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
    if (overlap < .0001f) return CollisionInfo(false);

    Vec2 circleClosestPoint = (smallest * colA->GetRadius());
    colA->contactPoints.push_back(circleClosestPoint);

        for (int i = 0; i < colB->GetVerts().size(); i++) {
            if (!IsInside(circleClosestPoint, colB->GetEdge(i))) break;
            if (i == colB->GetVerts().size() - 1) colB->contactPoints.push_back(circleClosestPoint);
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
    if (colInfo.collided) {
        Vec2 circleClosestPoint = (-colB->GetAxis() * colA->GetRadius());
        colA->contactPoints.push_back(circleClosestPoint);
    }
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.normal = colB->GetAxis();
    colInfo.depth = distance - colA->GetRadius();

    return colInfo;
}



CollisionInfo CollisionSolver::PolygonToPolygon(PolygonCollider* colA, PolygonCollider* colB) const
{
    if (colA == nullptr || colB == nullptr) return CollisionInfo(false);

    float overlap = FLT_MAX;
    Vec2 smallest;
    int index = -1;
    PolygonCollider* which = nullptr;
    PolygonCollider* notWhich = nullptr;

    Vec2 ax;
    Projection p1;
    Projection p2;

    std::vector<Vec2> vecAs = colA->GetVerts();
    for (Vec2& v : vecAs) {
        v += colA->GetPos();
    }

    std::vector<Vec2> vecBs = colB->GetVerts();
    for (Vec2& v : vecBs) {
        v += colB->GetPos();
    }

    for (int i = 0; i < colA->GetVerts().size(); i++) {
        ax = colA->GetAxis(i);

        p1 = ProjectOnAxis(ax, vecAs);
        p2 = ProjectOnAxis(ax, vecBs);

        if (!p1.Overlaps(p2)) {
            return CollisionInfo(false);
        }
        else {
            float o = p1.GetOverlap(p2);
            if (o < overlap) {
                overlap = o;
                smallest = ax;
                index = i;
                which = colA;
                notWhich = colB;
            }

        }

    }

    for (int i = 0; i < colB->GetVerts().size(); i++) {
        ax = colB->GetAxis(i);

        p1 = ProjectOnAxis(ax, vecAs);
        p2 = ProjectOnAxis(ax, vecBs);

        if (!p1.Overlaps(p2)) {
            return CollisionInfo(false);
        }
        else {
            float o = p2.GetOverlap(p1);
            if (o < overlap) {
                overlap = o;
                smallest = -ax;
                index = i;
                which = colB;
                notWhich = colA;
            }

        }

    }
    if (overlap < .0001f) return CollisionInfo(false);

    if (which && notWhich) {
        for (int repeat = 0; repeat < 2; repeat++) {
            for (int i = 0; i < notWhich->GetVerts().size(); i++) {
                for (int j = 0; j < which->GetVerts().size(); j++) {
                    if (!IsInside(notWhich->GetVert(i), which->GetEdge(j))) break;
                    if (j == which->GetVerts().size() - 1) which->contactPoints.push_back(notWhich->GetVert(i) - which->GetPos());
                }
            }
            PolygonCollider* s = which;
            which = notWhich;
            notWhich = s;
        }

    }



    if (overlap < .0001f) return CollisionInfo(false);


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
    std::vector<int> inds;
    for (int i = 0; i < colA->GetVerts().size(); i++) {
        float dist = Dot(colA->GetVert(i) + colA->GetPos(), colB->GetAxis()) + colB->GetDisplacement();
        if (dist <= overlap) {
            overlap = dist;
            inds.push_back(i);
        }
    }
    CollisionInfo colInfo;
    colInfo.collided = (overlap <= 0);
    if (colInfo.collided) {
        for (int i : inds) {
            if (Dot(colA->GetVert(i) + colA->GetPos(), colB->GetAxis()) + colB->GetDisplacement() < 0)
                colA->contactPoints.push_back(colA->GetVert(i) - colA->GetPos());
        }

    }
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
