#pragma once
#include "PhysicsObject.h"
class LineRenderer;

class PlaneCollider : public Collider {
public:
	PlaneCollider(Vec2 normal, float disp);
	PlaneCollider(PlaneCollider& p) = delete;
	PlaneCollider& operator=(PlaneCollider& other) = delete;
	void Rotate(float amnt) override {};
	Vec2 GetAxis() { return direction; }
	float GetDisplacement() { return displacement; }
private:
	Vec2 direction;
	float displacement;
};

class Plane : public PhysicsObject {
public:
	Plane(Vec2 normal, float disp, float elas = 1);

	void Draw(LineRenderer* lines) const override;
private:
	Vec2 direction;
};

