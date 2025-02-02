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
	for (PhysicsObject* c : objects) {
		delete c;
	}
	objects.clear();

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
	Vec2(-1, 1),
	Vec2(1, 1),
	Vec2(1, -1)
	};

	std::vector<Vec2> testVertsTwo = {
		Vec2(-1, -.5),
		Vec2(0, 1.5),
		Vec2(1, -.5)
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
	std::vector<Vec2> testVertsFour = {
		Vec2(-1, 0),
		Vec2(0, 1),
		Vec2(1, 0),
		Vec2(0, -1)
	};

	//objects.push_back(new Circle(Vec2(0, 0), 1));
	objects.push_back(new Polygon(Vec2(0, 0), 1, testVertsTwo));
	//objects.push_back(new Circle(Vec2(10, 10), 2));
	//objects.push_back(new Circle(Vec2(15, 4), 3));
	//objects.push_back(new Circle(Vec2(8, 2), 4));
	//objects.push_back(new Circle(Vec2(9, 2), 2));
	//objects.push_back(new Polygon(Vec2(8, 8), 1, testVertsThree));
	//objects.push_back(new Polygon(Vec2(12, 5), 1, testVerts));
	objects.push_back(new Polygon(Vec2(5, 0), 1, testVertsTwo));
}

void PhysicsSim::Update(float deltaTime)
{
	objects[0]->SetPosition(cursorPos);

	std::vector<CollisionInfo> allCollisions;

	//for (int i = 0; i < objects.size(); i++) {
	//	for (int j = i + 1; j < objects.size(); j++) {
	//		CollisionInfo check = drCollision.DetectCollision(objects[i]->collider, objects[j]->collider);
	//		allCollisions.push_back(check);
	//	}
	//}

	CollisionInfo check = drCollision.DetectCollision(objects[0]->collider, objects.back()->collider);
	allCollisions.push_back(check);
	for (CollisionInfo& collision : allCollisions) {

		//rCollision.ResolveCollision(collision);
		if (collision.collided) {
			lines->DrawLineWithArrow(collision.colliderB->GetPos(), collision.colliderB->GetPos() + collision.normal);
			lines->DrawLineWithArrow(collision.colliderA->GetPos(), collision.colliderA->GetPos() - collision.normal);
		}
	}

	for (PhysicsObject* c : objects) {
		c->Draw(lines);
	}


}

void PhysicsSim::OnLeftClick()
{
}
