#include <iostream>

#include "PhysicsSim.h"
#include "LineRenderer.h"
#include "Circle.h"
#include "Polygon.h"
#include "Plane.h"
#include "TextStream.h"

std::vector<Vec2> UNITCUBE = {
Vec2(-.5f, -.5f),
Vec2(-.5f, .5f),
Vec2(.5f, .5f),
Vec2(.5f, -.5f)
};

std::vector<Vec2> testVerts = {
Vec2(-1, -1),
Vec2(-1, 1),
Vec2(1, 1),
Vec2(1, -1)
};

std::vector<Vec2> testVertsBIG = {
Vec2(-2, -2),
Vec2(-2, 2),
Vec2(2, 2),
Vec2(2, -2)
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
std::vector<Vec2> testVertsFive = {
	Vec2(-5, 1),
	Vec2(5, 1),
	Vec2(5, -1),
	Vec2(-5, -1)
};

std::vector<Vec2> testVertsHayden = {
	Vec2(27, -9),
	Vec2(35, -12),
	Vec2(36, -14),
	Vec2(37, -18),
	Vec2(38, -23),
	Vec2(39, -33),
	Vec2(40, -83),
};

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



	//objects.push_back(new Circle(Vec2(0, 0), 1));
	objects.push_back(new Polygon(Vec2(0, 5), UNITCUBE));
	objects.push_back(new Polygon(Vec2(0, -5), UNITCUBE, STATIC));
	objects.push_back(new Plane(Vec2(-1, 1), 5));
	objects.push_back(new Plane(Vec2(1, 1), 5));
	objects.push_back(new Plane(Vec2(-1, -1), 5));
	objects.push_back(new Plane(Vec2(1, -1), 5));
	//objects.push_back(new Circle(Vec2(10, 10), 2));
	//objects.push_back(new Circle(Vec2(15, 4), 3));
	//objects.push_back(new Circle(Vec2(8, 2), 4, STATIC));
	//objects.push_back(new Circle(Vec2(9, 2), 2));
	//objects.push_back(new Polygon(Vec2(12, 5), 1, testVerts));
	//objects.push_back(new Polygon(Vec2(5, 0), 1, testVertsFour));
	//objects.push_back(new Polygon(Vec2( - 10, 0), 1, testVertsFive, STATIC));

	for (PhysicsObject* object : objects) {
		std::cout << object->GUID << '\n';
	}
}

void PhysicsSim::Update(float deltaTime)
{

	objects[0]->ApplyForce(Vec2(0, -1) * deltaTime);
	std::vector<CollisionInfo> allCollisions;

	for (int w = 0; w < 3; w++) {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				CollisionInfo check = drCollision.DetectCollision(objects[i]->collider, objects[j]->collider);
				allCollisions.push_back(check);
			}
		}
		for (CollisionInfo& collision : allCollisions) {
			drCollision.ResolveCollision(collision);
		}
		allCollisions.clear();
	}




	for (PhysicsObject* c : objects) {
		c->Update(deltaTime);
		c->Draw(lines);
		c->collider->EndTick();
	}

}

void PhysicsSim::OnLeftClick()
{
}
