#pragma once
#include "Polygon.h"
#include "Circle.h"

class Bumper : public Circle {
public:
	Bumper(Vec2 pos, float bumpStrength) : Circle(pos, 1, 1, DYNAMIC), strength(bumpStrength) { this->inverseMass = 0; this->collider->SetInvMass(0); };

	float strength;
	bool drawBounce;
	float bounceDrawSize = 1.2;
	int framesActive = 0;
	Vec2 thisBounceDir = Vec2(0, 0);
	void Update(float dt) override;
	void Notify(PhysicsObject* other) override;
	void Draw(LineRenderer* lines) const override;

};

static std::vector<Vec2> bladeVerts = {
	Vec2(-1, -.5),
	Vec2(0, 1.5),
	Vec2(1, -.5)
};

class Blade : public Polygon {
public:
	Blade(Vec2 pos) : Polygon(pos, bladeVerts, 0) {};

	void Notify(PhysicsObject* other) override;
	void Draw(LineRenderer* lines) const override;


};
class BladeSpinners : public PhysicsObject{
public:
	BladeSpinners(Vec2 pos, int count, float rotSpeed, std::vector<PhysicsObject*>& queue);
	float rotationSpeed;
	std::vector<Blade*> blades;
	void Update(float dt) override;
	void Draw(LineRenderer* lines) const override;
	void Unload() override;
	
};
