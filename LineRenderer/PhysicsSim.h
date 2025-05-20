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
	std::vector<PhysicsObject*> objectQueue;
	void Initialise() override;

	void Update(float deltaTime) override;

	//Geometry Painter
	std::vector<Vec2> storedVerts;
	void OnLeftClick() override;
	void OnRightClick() override;
	
	void OnKeyPress(Key key) override;
};