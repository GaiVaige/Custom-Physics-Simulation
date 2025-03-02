#include "CollisionSolver.h"
#include "Collider.h"
#include "Circle.h"
#include "Polygon.h"
#include "Plane.h"
#include "Constraint.h"
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
    
    float totalInvMass = colInfo.colliderA->GetParent()->inverseMass + colInfo.colliderB->GetParent()->inverseMass;
    if (totalInvMass == 0) {
        return;
    }


    Vec2 velA = colInfo.colliderA->GetParent()->GetVelocity();
    Vec2 velB = colInfo.colliderB->GetParent()->GetVelocity();
    float elas = (colInfo.colliderA->GetParent()->elasticity + colInfo.colliderB->GetParent()->elasticity) / 2.f;
    Vec2 aSpot;
    if (colInfo.colliderA->GetParent()->constraint != nullptr) {
        aSpot = colInfo.colliderA->GetParent()->constraint->position;
    }
    else {
        aSpot = colInfo.colliderA->GetPos();
    }
    Vec2 aAv = colInfo.contactPoint - aSpot;
    float arcA = Dot(colInfo.normal.GetRotatedBy90(), aAv);
    float effmassA = (colInfo.colliderA->GetParent()->inverseMass + (pow(arcA, 2) * colInfo.colliderA->GetParent()->inverseMomentOfInertia));
    Vec2 bSpot;
    if (colInfo.colliderB->GetParent()->constraint != nullptr) {
        bSpot = colInfo.colliderB->GetParent()->constraint->position;
    }
    else {
        bSpot = colInfo.colliderB->GetPos();
    }
    Vec2 bAv = colInfo.contactPoint - bSpot;
    float arcB = Dot(colInfo.normal.GetRotatedBy90(), bAv);
    float effmassB = (colInfo.colliderB->GetParent()->inverseMass + (pow(arcB, 2) * colInfo.colliderB->GetParent()->inverseMomentOfInertia));


    Vec2 relVelA = (colInfo.colliderA->GetParent()->GetVelocityAt(aAv + colInfo.colliderA->GetPos()));
    Vec2 relVelB = (colInfo.colliderB->GetParent()->GetVelocityAt(bAv + colInfo.colliderB->GetPos()));
    
    Vec2 totalRelVel = relVelA + relVelB;

    float j = -(1 + elas) * Dot((relVelB - relVelA), colInfo.normal) / (effmassB + effmassA);

    float aAdd = 0;
    float bAdd = 0;
    if (colInfo.colliderA->GetParent()->constraint == nullptr) {
        if (colInfo.colliderB->GetParent()->constraint != nullptr) {
            bAdd = colInfo.colliderB->GetParent()->inverseMass;
        }
        Vec2 AOffset = (-colInfo.normal * colInfo.depth * (colInfo.colliderA->GetParent()->inverseMass + bAdd / totalInvMass));
        colInfo.colliderA->Move(AOffset);
    }
    if (colInfo.colliderB->GetParent()->constraint == nullptr) {
        if (colInfo.colliderA->GetParent()->constraint != nullptr) {
            aAdd = colInfo.colliderA->GetParent()->inverseMass;
        }
        Vec2 BOffset = colInfo.normal * colInfo.depth * (colInfo.colliderB->GetParent()->inverseMass + aAdd / totalInvMass);
        colInfo.colliderB->Move(BOffset);
    }


    //colInfo.colliderA->GetParent()->ApplyImpulse(-colInfo.normal * j);
    colInfo.colliderA->GetParent()->ApplyForceAt(-colInfo.normal * j, aAv);
    colInfo.colliderA->GetParent()->CollisionEvent(colInfo.colliderB->GetParent());
    //colInfo.colliderB->GetParent()->ApplyImpulse(colInfo.normal * j);
    colInfo.colliderB->GetParent()->ApplyForceAt(colInfo.normal * j, bAv);
    colInfo.colliderB->GetParent()->CollisionEvent(colInfo.colliderA->GetParent());

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
    CollisionInfo colInfo;
    float distance = (colB->GetPos() - colA->GetPos()).GetMagnitude();
    float sumOfRadii = (colA->GetRadius() + colB->GetRadius());
    if (distance > sumOfRadii) {
        return CollisionInfo(false);
    }



    Vec2 normal = (colB->GetPos() - colA->GetPos()).GetNormalised();
    colInfo.contactPoint = (normal * colA->GetRadius() + colA->GetPos());
    float penAmnt = sumOfRadii - distance;
    colInfo.collided = true;
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.normal = normal;
    colInfo.depth = penAmnt;
    return colInfo;
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
    std::vector<Vec2> bTranslatedPoints;

    for (Vec2 v : colB->GetVerts()) {
        bTranslatedPoints.push_back(v.GetRotatedBy(colB->GetParent()->GetOrientation()) + colB->GetPos());
    }

    for (int i = 0; i < colB->GetVerts().size(); i++) {
        
        Projection p2 = ProjectOnAxis(axis, bTranslatedPoints);

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
        Projection p2 = ProjectOnAxis(ax, bTranslatedPoints);

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
    if (overlap < .0001f) return CollisionInfo(false);

    Vec2 circleClosestPoint = (smallest * colA->GetRadius() + colA->GetPos());
    CollisionInfo colInfo;
    colInfo.contactPoint = (circleClosestPoint);


    colInfo.collided = true;
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.depth = overlap;
    colInfo.normal = smallest;
    return colInfo;


}

CollisionInfo CollisionSolver::CircleToPlane(CircleCollider* colA, PlaneCollider* colB) const
{
    float distance = Dot(colA->GetPos(), colB->GetNormal()) + colB->GetDisplacement();

    CollisionInfo colInfo;
    colInfo.collided = distance < colA->GetRadius();
    if (colInfo.collided) {
        Vec2 circleClosestPoint = (-(colB->GetNormal()) * colA->GetRadius());
        colInfo.contactPoint = (circleClosestPoint + colA->GetPos());
    }
    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.normal = colB->GetNormal();
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
    std::vector<Vec2> whichPoints;
    std::vector<Vec2> notWhichPoints;

    std::vector<Vec2> aTranslatedPoints;

    for (Vec2 v : colA->GetVerts()) {
        aTranslatedPoints.push_back(v.GetRotatedBy(colA->GetParent()->GetOrientation()) + colA->GetPos());
    }

    std::vector<Vec2> bTranslatedPoints;

    for (Vec2 v : colB->GetVerts()) {
        bTranslatedPoints.push_back(v.GetRotatedBy(colB->GetParent()->GetOrientation()) + colB->GetPos());
    }

    Vec2 ax;
    Projection p1;
    Projection p2;
    for (int i = 0; i < colA->GetVerts().size(); i++) {
        ax = colA->GetAxis(i);

        p1 = ProjectOnAxis(ax, aTranslatedPoints);
        p2 = ProjectOnAxis(ax, bTranslatedPoints);

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
                whichPoints = aTranslatedPoints;
                notWhich = colB;
                notWhichPoints = bTranslatedPoints;
            }

        }

    }

    for (int i = 0; i < colB->GetVerts().size(); i++) {
        ax = colB->GetAxis(i);

        p1 = ProjectOnAxis(ax, aTranslatedPoints);
        p2 = ProjectOnAxis(ax, bTranslatedPoints);

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
                whichPoints = bTranslatedPoints;
                notWhichPoints = aTranslatedPoints;
            }

        }

    }
    if (overlap < .0001f) return CollisionInfo(false);
    std::vector<Vec2> cPnts;

    if (which && notWhich) {
        for (int repeat = 0; repeat < 2; repeat++) {
            for (int i = 0; i < notWhich->GetVerts().size(); i++) {
                for (int j = 0; j < which->GetVerts().size(); j++) {
                    if (!IsInside(notWhichPoints[i], which->GetEdge(j))) break;
                    if (j == which->GetVerts().size() - 1) cPnts.push_back(notWhichPoints[i]);
                }
            }
            PolygonCollider* s = which;
            std::vector<Vec2> sV = whichPoints;
            which = notWhich;
            whichPoints = notWhichPoints;
            notWhich = s;
            notWhichPoints = sV;
        }

    }

    Vec2 av = Vec2(0, 0);
    for (Vec2 v : cPnts) {
        av += v;
    }
    if (!cPnts.empty()) {
        av /= cPnts.size();
    }




    CollisionInfo colInfo;
    colInfo.contactPoint = av;
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
    int ind = -1;
    for (int i = 0; i < colA->GetVerts().size(); i++) {
        float dist = Dot(colA->GetVert(i), colB->GetNormal()) + colB->GetDisplacement();
        if (dist <= overlap) {
            overlap = dist;
            ind = i;
        }
    }
    CollisionInfo colInfo;
    colInfo.collided = (overlap <= 0);
    
    if (colInfo.collided) {
        colInfo.contactPoint = colA->GetVert(ind);
    }

    colInfo.colliderA = colA;
    colInfo.colliderB = colB;
    colInfo.normal = colB->GetNormal();
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
