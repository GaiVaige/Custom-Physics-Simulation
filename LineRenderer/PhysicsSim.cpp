#include "PhysicsSim.h"
#include "LineRenderer.h"
#include "Circle.h"
PhysicsSim::PhysicsSim()
{
	appInfo.appName = "Testing Out This Physics Thing";
}

PhysicsSim::~PhysicsSim()
{
}

void PhysicsSim::Initialise()
{
	circles.push_back(new Circle(Vec2(0, 0), .5f));
	circles.push_back(new Circle(Vec2(0, 0), .2f));

}

void PhysicsSim::Update(float deltaTime)
{
	circles[0]->SetPosition(cursorPos);

	CollisionInfo check = drCollision.CheckForCollision(circles[0]->collider, circles[1]->collider);

	lines->DrawText(std::to_string(check.collided), Vec2(), .4f);
	for (Circle* c : circles) {
		c->Draw(lines);
	}
}

void PhysicsSim::OnLeftClick()
{
}
