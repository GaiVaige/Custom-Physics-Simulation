#include "Polygon.h"
#include "LineRenderer.h"
PolygonCollider::PolygonCollider(Vec2& centre, float mass)
{
	position = centre;
	inverseMass = 1.f/mass;
}

PolygonCollider::PolygonCollider(Vec2& centre, float mass, Vec2* v, int f)
{
	shape = POLYGON;
	position = centre;
	inverseMass = 1.f / mass;
	verts = v;
	faces = f;
	axes = CalcNormals(verts);
}

PolygonCollider::~PolygonCollider()
{
	if (verts != nullptr) {
		delete verts;
	}
}

void PolygonCollider::DebugDrawAxis(LineRenderer* lines)
{
	lines->SetColour(Colour::BLUE);
	for (int i = 0; i < faces; i++) {
		lines->AddPointToLine(position);
		lines->AddPointToLine(axes[i] + position);
		lines->FinishLineLoop();
	}
	lines->SetColour(Colour::WHITE);
}

Vec2* PolygonCollider::CalcNormals(Vec2* vertices)
{
	Vec2* norms = new Vec2[faces];
	for (int i = 0; i < faces; i++) {
		int j = i + 1;
		if (j >= faces)	j = 0;
		Vec2 lineSeg = verts[j] - verts[i];
		norms[i] = Vec2(-lineSeg.y, lineSeg.x).Normalise();
	}

	return norms;
}

Vec2 PolygonCollider::GetVert(int i)
{
	Vec2 v = verts[i];
	return v;
}

Polygon::Polygon(Vec2 pos, float mass, Vec2* v, int f)
{
	position = pos;
	verts = new Vec2[f];
	for (int i = 0; i < f; i++) {
		Vec2 vc = v[i];
		verts[i] = Vec2(vc);
	}
	faces = f;
	collider = new PolygonCollider(pos, mass, v, faces);
	collider->SetPos(pos);
}

Polygon::~Polygon()
{
	delete collider;
}

void Polygon::Draw(LineRenderer* lines)
{
	for (int i = 0; i < faces; i++) {
		lines->AddPointToLine(verts[i] + position);
	}
	lines->FinishLineLoop();

	PolygonCollider* p = static_cast<PolygonCollider*>(collider);

	//p->DebugDrawAxis(lines);
}
