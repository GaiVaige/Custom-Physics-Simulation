#include "Polygon.h"
#include "LineRenderer.h"
#include <string>
#include <iostream>
PolygonCollider::PolygonCollider(Vec2& centre, float mass)
{
	position = centre;
	inverseMass = 1.f/mass;
}

PolygonCollider::PolygonCollider(Vec2& centre, float mass, std::vector<Vec2> v)
{
	shape = POLYGON;
	position = centre;
	inverseMass = 1.f / mass;
	for (Vec2 vec : v) {
		verts.push_back(vec);
	}
	baseVerts = verts;
	CalcNormals(verts);
}

PolygonCollider::~PolygonCollider()
{
}

void PolygonCollider::DebugDrawAxis(LineRenderer* lines)
{

	//for (Vec2 p : contactPoints) {
	//	lines->DrawCircle(p + position, .1, Colour::MAGENTA);
	//}
}

void PolygonCollider::Rotate(float amnt)
{
	std::vector<Vec2> rotatedNorms;
	for (Vec2 v : verts) {
		rotatedNorms.push_back(v.GetRotatedBy(GetParent()->GetOrientation()));
	}
	CalcNormals(rotatedNorms);
}

void PolygonCollider::CalcNormals(std::vector<Vec2>& vertices)
{
	axes.clear();
	edges.clear();
	for (int i = 0; i < vertices.size(); i++) {
		int j = i + 1;
		if (j >= vertices.size())	j = 0;
		Vec2 lineSeg = vertices[j] - vertices[i];
		axes.push_back(Vec2(-lineSeg.y, lineSeg.x).GetNormalised());
		edges.push_back(std::pair<Vec2, Vec2>(vertices[j], vertices[i]));
	}


}

Vec2& PolygonCollider::SetPos(Vec2& pos)
{
	for (std::pair<Vec2, Vec2>& v : edges) {
		v.first -= position;
		v.second -= position;
	}
	this->Collider::SetPos(pos);
	for (std::pair<Vec2, Vec2>& v : edges) {
		v.first += position;
		v.second += position;
	}

	return pos;
}

Vec2 PolygonCollider::GetVert(int i)
{
	Vec2 v = verts[i].GetRotatedBy(GetParent()->GetOrientation());
	v += position;
	return v;
}

Polygon::Polygon(Vec2 pos, std::vector<Vec2>& v, float elas, PHYSICSTYPE t)
{
	position = pos;
	elasticity = elas;
	for (Vec2 vec : v) {
		Vec2 vc = Vec2(vec.x, vec.y);
		verts.push_back(vc);
	}
	inverseMass = CalculateMass();
	collider = new PolygonCollider(pos, inverseMass, v);
	inverseMass = 1.f / inverseMass;
	collider->SetParent(this);
	collider->SetPos(pos);
	type = t;
	if (type == STATIC) {
		collider->SetInvMass(0);
		inverseMass = 0;
		inverseMomentOfInertia = 0;
	}
}

void Polygon::Draw(LineRenderer* lines) const
{
	for (int i = 0; i < verts.size(); i++) {
		lines->AddPointToLine(verts[i].GetRotatedBy(orientation) + position);
	}
	lines->FinishLineLoop();

}

void Polygon::Rotate(float amnt)
{
	orientation += amnt;
	collider->Rotate(amnt);
	up.RotateBy(amnt);
	right.RotateBy(amnt);
	PhysicsObject::Rotate(amnt);
}

bool Polygon::IsInside(Vec2 p, std::vector<Vec2> verts) {

	for (int i = 0; i < verts.size(); i++) {
		int j = i + 1;
		if (j == verts.size()) j = 0;
		Vec2 check = verts[j] - verts[i];
		if ((PseudoCross(check, p) - PseudoCross(verts[j], verts[i])) <= 0.0f){
			continue;
		};
		return false;

	}
	return true;

}

bool operator!=(Vec2& v, Vec2& v2) {
	float a = abs(v.x - v2.x);
	float b = abs(v.y - v2.y);

	return a + b < .01;
}

float Polygon::CalculateMass()
{

	Vec2 min, max;
	for (Vec2 v : verts) {
		if (v.x < min.x) min.x = v.x;
		if (v.y < min.y) min.y = v.y;
		if (v.x > max.x) max.x = v.x;
		if (v.y > max.y) max.y = v.y;
	}
	Vec2 start = Vec2(min.x, min.y);
	Vec2 end = Vec2(max.x, max.y);
	Vec2 dispAmnt = Vec2(.25, 0);
	Vec2 vertDispAmnt = Vec2(0, .25);
	float baseFloatWeight = .1f;

	std::vector<Vec2> containedVectors;
	Vec2 track = start;
	bool finished = false;
	while(!finished) {
		if (IsInside(track, verts)) {
			containedVectors.push_back(track);
		}

		if (track.x >= max.x) {
			track += vertDispAmnt;
			track.x = min.x;
			if (track.y > max.y) finished = true;
		}
		else {
			track += dispAmnt;
		}
	}
	Vec2 overallPos = Vec2(0,0);
	for (Vec2 p : containedVectors) {
		overallPos += p;
	}
	overallPos /= containedVectors.size();
	centreOfMassDisplacement = overallPos;
	inverseMomentOfInertia = 1.0f / CalculateMomentOfInertia(position + centreOfMassDisplacement, containedVectors, baseFloatWeight);
	


	return containedVectors.size() * baseFloatWeight;
}
