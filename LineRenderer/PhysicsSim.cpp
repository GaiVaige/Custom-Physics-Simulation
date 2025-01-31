#include "PhysicsSim.h"
#include "LineRenderer.h"
#include "Circle.h"
#include "Polygon.h"
PhysicsSim::PhysicsSim()
{
	appInfo.appName = "Testing Out This Physics Thing";
}

PhysicsSim::~PhysicsSim()
{
}

Vec2 testVerts[4]{
	Vec2(-1, -1),
	Vec2(1, -1),
	Vec2(1, 1),
	Vec2(-1, 1)
};

void PhysicsSim::Initialise()
{
	//circles.push_back(new Circle(Vec2(0, 0), .5f));
	//for (int i = 0; i < 1; i++) {
	//	for (int ii = 0; ii < 1; ii++) {
	//		circles.push_back(new Circle(Vec2(i - 5, ii - 5), 5.f));
	//	}
	//}

	polys.push_back(new Polygon(Vec2(0, 0), 10, testVerts, 4));
	polys.push_back(new Polygon(Vec2(0, 0), 10, testVerts, 4));

}

void PhysicsSim::Update(float deltaTime)
{
	polys[0]->SetPosition(cursorPos);

	CollisionInfo check = drCollision.DetectCollision(polys[0]->collider, polys[1]->collider);

	if (check.collided) {
		lines->DrawText("Hello", Vec2(0, 0), 1);
	}
	//for (Circle* c : circles) {
	//	c->Draw(lines);
	//}

	for (Polygon* p : polys) {
		p->Draw(lines);
	}
	polys[0]->Draw(lines);
}

void PhysicsSim::OnLeftClick()
{
}
