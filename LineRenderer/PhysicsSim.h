#pragma once
#include "Application.h"
#include "Maths.h"
#include <vector>
#include "CollisionSolver.h"
class LineRenderer;

class Circle;


class PhysicsSim : public Application {
private:

	float fixedUpdateStepThreshold = .2f;
	float fixedUpdateTimer = .0f;

public:
	PhysicsSim();
	~PhysicsSim();
	CollisionSolver drCollision;
	std::vector<Circle*> circles;

	void Initialise() override;

	void Update(float deltaTime) override;

	void OnLeftClick() override;
};