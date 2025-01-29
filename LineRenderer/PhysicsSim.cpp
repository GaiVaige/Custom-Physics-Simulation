#include "PhysicsSim.h"

PhysicsSim::PhysicsSim()
{
}

PhysicsSim::~PhysicsSim()
{
}

void PhysicsSim::Initialise()
{
}

void PhysicsSim::Update(float deltaTime)
{
	//end with incrementing fixedupdatetimerval
	fixedUpdateTimer += deltaTime;
	if (fixedUpdateTimer >= fixedUpdateStepThreshold) {
		FixedUpdate();
	}
}

void PhysicsSim::FixedUpdate()
{
	//end with resetting down the timer
	fixedUpdateTimer -= fixedUpdateStepThreshold;
}

void PhysicsSim::OnLeftClick()
{
}
