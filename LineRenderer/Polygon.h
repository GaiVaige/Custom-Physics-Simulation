#pragma once
#include "PhysicsObject.h"
#include <vector>
class LineRenderer;
class PolygonCollider : public Collider {
public:
	PolygonCollider(Vec2& centre, float mass);
	PolygonCollider(Vec2& centre, float mass, std::vector<Vec2> v);
	~PolygonCollider();
	PolygonCollider(PolygonCollider& c) = delete;
	PolygonCollider& operator=(PolygonCollider& c) = delete;
	void DebugDrawAxis(LineRenderer* lines);
	void CalcNormals(std::vector<Vec2>& vertices);

	Vec2& SetPos(Vec2& pos) override;

	Vec2 GetAxis(int i) { return axes[i]; }
	Vec2 GetVert(int i);
	std::vector<Vec2>& GetVerts() { return verts; }
	std::vector<Vec2> contactPoints;
private:
	std::vector<Vec2> baseVerts;
	std::vector<Vec2> verts;
	std::vector<Vec2> axes;
};

class Polygon : public PhysicsObject {
public:
	Polygon(Vec2 pos, float mass, std::vector<Vec2>& v, PHYSICSTYPE t = PHYSICSTYPE::DYNAMIC);
	void Draw(LineRenderer* lines) const override;

private:
	std::vector<Vec2> verts;

};
