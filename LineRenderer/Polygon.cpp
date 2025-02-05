#include "Polygon.h"
#include "LineRenderer.h"
#include <string>
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
	lines->SetColour(Colour::BLUE);
	for (Vec2 v : contactPoints) {
		lines->DrawCircle(v, .2);
	}
	lines->SetColour(Colour::WHITE);
}

void PolygonCollider::CalcNormals(std::vector<Vec2>& vertices)
{
	for (int i = 0; i < vertices.size(); i++) {
		int j = i + 1;
		if (j >= vertices.size())	j = 0;
		Vec2 lineSeg = verts[j] - verts[i];
		axes.push_back(Vec2(-lineSeg.y, lineSeg.x).GetNormalised());
		edges.push_back(std::pair<Vec2, Vec2>(verts[j] + position, verts[i] + position));
	}


}

Vec2& PolygonCollider::SetPos(Vec2& pos)
{
	for (std::pair<Vec2, Vec2>& v : edges) {
		v.first -= position;
		v.second -= position;
	}

	this->Collider::SetPos(pos);
	verts.clear();

	for (Vec2 v : baseVerts) {
		verts.push_back(v + pos);
	}

	for (std::pair<Vec2, Vec2>& v : edges) {
		v.first += position;
		v.second += position;
	}

	return pos;
}

Vec2 PolygonCollider::GetVert(int i)
{
	Vec2 v = verts[i];
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
	collider->SetParent(this);
	collider->SetPos(pos);
	type = t;
	if (type == STATIC) {
		collider->SetInvMass(0);
	}
}

void Polygon::Draw(LineRenderer* lines) const
{
	for (int i = 0; i < verts.size(); i++) {
		lines->AddPointToLine(verts[i] + position);
	}
	lines->FinishLineLoop();
	lines->DrawText(std::to_string(linearVelocity.GetMagnitude()), position, 1);
	//lines->DrawLineWithArrow(position, position + velocity);

}

void Polygon::Rotate(float amnt)
{
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
	float baseFloatWeight = .04f;

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

	momentOfIntertia = CalculateMomentOfInertia(position, containedVectors, baseFloatWeight);
	
	momentOfIntertia = CalculateMomentOfInertia(position, containedVectors, baseFloatWeight);


	return containedVectors.size() * baseFloatWeight;
}
