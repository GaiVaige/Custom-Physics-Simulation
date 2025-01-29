#include "PhysicsObject.h"
#include <chrono>

PhysicsObject::PhysicsObject()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	GUID = nanos;
}

PhysicsObject::~PhysicsObject()
{
	delete collider;
}
