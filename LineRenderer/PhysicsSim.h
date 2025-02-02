#pragma once
#include "Application.h"
#include "Maths.h"
#include <vector>
#include "CollisionSolver.h"
class LineRenderer;

class Circle;
class Polygon;

class PhysicsSim : public Application {
private:

public:
	PhysicsSim();
	~PhysicsSim();
	CollisionSolver drCollision;
	std::vector<PhysicsObject*> objects;
	void Initialise() override;

	void Update(float deltaTime) override;

	void OnLeftClick() override;
};