#include "Polygon.h"
#include "LineRenderer.h"
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
	for (int i = 0; i < axes.size() ; i++) {
		Vec2 v = GetAxis(i);
		int j = i + 1;
		if (j >= verts.size())	j = 0;
		Vec2 lineSeg = verts[j] - verts[i];
		lineSeg /= 2;

		lines->DrawLineWithArrow(verts[i] + lineSeg, verts[i] + lineSeg + v);

	}
	lines->SetColour(Colour::WHITE);
}

void PolygonCollider::CalcNormals(std::vector<Vec2>& vertices)
{
	for (int i = 0; i < vertices.size(); i++) {
		int j = i + 1;
		if (j >= vertices.size())	j = 0;
		Vec2 lineSeg = verts[j] - verts[i];
		axes.push_back(Vec2(-lineSeg.y, lineSeg.x).Normalise());
	}

}

Vec2& PolygonCollider::SetPos(Vec2& pos)
{
	this->Collider::SetPos(pos);
	verts.clear();

	for (Vec2 v : baseVerts) {
		verts.push_back(v + pos);
	}

	return pos;
}

Vec2 PolygonCollider::GetVert(int i)
{
	Vec2 v = verts[i];
	return v;
}

Polygon::Polygon(Vec2 pos, float mass, std::vector<Vec2>& v)
{
	position = pos;
	for (Vec2 vec : v) {
		Vec2 vc = Vec2(vec.x, vec.y);
		verts.push_back(vc);
	}
	collider = new PolygonCollider(pos, mass, v);
	collider->SetParent(this);
	collider->SetPos(pos);
}

void Polygon::Draw(LineRenderer* lines)
{
	for (int i = 0; i < verts.size(); i++) {
		lines->AddPointToLine(verts[i] + position);
	}
	lines->FinishLineLoop();

	PolygonCollider* p = static_cast<PolygonCollider*>(collider);

	p->DebugDrawAxis(lines);
}
