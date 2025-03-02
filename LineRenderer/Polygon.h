#pragma once
#include "PhysicsObject.h"
#include <vector>
class LineRenderer;
class PolygonCollider : public Collider {
public:
	PolygonCollider(Vec2& centre, float mass);
	PolygonCollider(Vec2& centre, float mass, std::vector<Vec2> v);
	PolygonCollider(PolygonCollider& c) = delete;
	PolygonCollider& operator=(PolygonCollider& c) = delete;
	void CalcNormals(std::vector<Vec2>& vertices);


	void SetPos(Vec2& pos) override;
	void Rotate(float amnt) override;

	Vec2 GetAxis(int i) { return axes[i]; }
	Vec2 GetVert(int i);
	std::pair<Vec2, Vec2> GetEdge(int i) { return edges[i]; }
	std::vector<Vec2>& GetVerts() { return verts; }
	void DebugDrawAxis(LineRenderer* lines);
protected:
private:
	std::vector<Vec2> verts;
	std::vector<Vec2> axes;
	std::vector<std::pair<Vec2, Vec2>> edges;
};

class Polygon : public PhysicsObject {
public:
	Polygon(Vec2 pos, std::vector<Vec2>& v, float elasticity, PHYSICSTYPE t = PHYSICSTYPE::DYNAMIC);
	void Draw(LineRenderer* lines) const override;

	void Rotate(float amnt) override;

	float CalculateMass() override;
	bool IsInside(Vec2 p, std::vector<Vec2> verts);
	std::vector<Vec2> GetVerts() { return verts; }
protected:
	std::vector<Vec2>& GetVertsByRef() { return verts; }
private:
	std::vector<Vec2> verts;

};
