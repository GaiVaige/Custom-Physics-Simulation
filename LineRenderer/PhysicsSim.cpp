#include "PhysicsSim.h"
#include "LineRenderer.h"
#include "Circle.h"
#include "Polygon.h"
#include "Key.h"
PhysicsSim::PhysicsSim()
{
	appInfo.appName = "Testing Out This Physics Thing";
}

PhysicsSim::~PhysicsSim()
{
	for (Circle* c : circles) {
		delete c;
	}
	circles.clear();

	for (Polygon* p : polys) {
		delete p;
	}
	polys.clear();
}



void PhysicsSim::Initialise()
{
	//circles.push_back(new Circle(Vec2(0, 0), .5f));
	//for (int i = 0; i < 1; i++) {
	//	for (int ii = 0; ii < 1; ii++) {
	//		circles.push_back(new Circle(Vec2(i - 5, ii - 5), 5.f));
	//	}
	//}

	std::vector<Vec2> testVerts ={
	Vec2(-1, -1),
	Vec2(1, -1),
	Vec2(1, 1),
	Vec2(-1, 1)
	};

	std::vector<Vec2> testVertsTwo = {
		Vec2(0, 1),
		Vec2(1, 0),
		Vec2(-1, 0)
	};

	std::vector<Vec2> testVertsThree = {
		Vec2(0, -1),
		Vec2(-.6, -.6),
		Vec2(-1, 0),
		Vec2(-.6, .6),
		Vec2(0, 1),
		Vec2(.6, .6),
		Vec2(1, 0),
		Vec2(.6, -.6)
	};

	//polys.push_back(new Polygon(Vec2(0, 0), 100, testVertsTwo));
	//polys.push_back(new Polygon(Vec2(0, 0), 100, testVertsThree));
	polys.push_back(new Polygon(Vec2(0, 0), 5, testVerts));
	circles.push_back(new Circle(Vec2(0, 0), 1));
}

void PhysicsSim::Update(float deltaTime)
{
	circles[0]->SetPosition(cursorPos);

	std::vector<CollisionInfo> allCollisions;

	CollisionInfo check = drCollision.DetectCollision(circles[0]->collider, polys[0]->collider);
	allCollisions.push_back(check);


	for (CollisionInfo& collision : allCollisions) {
		drCollision.ResolveCollision(collision);
	}

	for (Circle* c : circles) {
		c->Draw(lines);
	}

	for (Polygon* p : polys) {
		p->Draw(lines);
	}

}

void PhysicsSim::OnLeftClick()
{
}
