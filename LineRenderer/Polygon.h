#pragma once
#include "PhysicsObject.h"
#include "Vec2.h"
#include <array>
class LineRenderer;
class PolygonCollider : public Collider {
public:
	PolygonCollider(Vec2& centre, float mass);
	PolygonCollider(Vec2& centre, float mass, Vec2* v, int f);
	~PolygonCollider();
	PolygonCollider(PolygonCollider& c) = delete;
	PolygonCollider& operator=(PolygonCollider& c) = delete;
	void DebugDrawAxis(LineRenderer* lines);
	Vec2* CalcNormals(Vec2* vertices);

	Vec2 GetAxis(int i) { return axes[i]; }
	Vec2 GetVert(int i);
	Vec2* GetVerts() { return verts; }
	int faces;
private:
	Vec2* verts = nullptr;
	Vec2* axes = nullptr;
};

class Polygon : public PhysicsObject {
public:
	Polygon(Vec2 pos, float mass, Vec2* v, int faces);
	~Polygon();
	void Draw(LineRenderer* lines) override;

private:
	Vec2* verts = nullptr;
	int faces = 0;
};
