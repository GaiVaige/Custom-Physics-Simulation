#pragma once
#include "Application.h"
#include "Maths.h"

class PhysicsSim : public Application {
private:

	float fixedUpdateStepThreshold = .2f;
	float fixedUpdateTimer = .0f;

public:
	PhysicsSim();
	~PhysicsSim();

	void Initialise() override;

	void Update(float deltaTime) override;

	void FixedUpdate();

	void OnLeftClick() override;
};