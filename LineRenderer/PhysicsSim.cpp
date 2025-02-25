#include <iostream>

#include "PhysicsSim.h"
#include "LineRenderer.h"
#include "Circle.h"
#include "Polygon.h"
#include "Plane.h"
#include "TextStream.h"
#include "Key.h"

#include "ApplicationHarness.h"
#include "Launcher.h"
#include "Target.h"
#pragma region ShapeVerts
std::vector<Vec2> UNITCUBE = {
Vec2(-.5f, -.5f),
Vec2(-.5f, .5f),
Vec2(.5f, .5f),
Vec2(.5f, -.5f)
};

std::vector<Vec2> RECTANGLE = {
Vec2(-2, -.5f),
Vec2(-2, .5f),
Vec2(2, .5f),
Vec2(2, -.5f)
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
#pragma endregion



PhysicsSim::PhysicsSim()
{
	appInfo.appName = "Great Steal Caro";
	appInfo.horizontalResolution = 1280;
	appInfo.verticalResolution = 720;
	appInfo.camera.disable = true;
	cameraCentre = Vec2(0, 0);
	cameraHeight = 45;
	appInfo.grid.show = false;
	appInfo.grid.extent = 50;
}

PhysicsSim::~PhysicsSim()
{
	for (PhysicsObject* c : objects) {
		delete c;
	}
	objects.clear();

}
Launcher* playerLauncher;
void PhysicsSim::Initialise()
{
	playerLauncher = new Launcher(Vec2(0, -20), Vec2(0, 1.5));
	objects.push_back(playerLauncher);
	playerLauncher->sceneObjects = &objects;
	//objects.push_back(new Circle(Vec2(-3, 10), 1, .5));
	//objects 1 through 
	objects.push_back(new Polygon(Vec2(3, -5), RECTANGLE, .5));
	//objects.push_back(new Polygon(Vec2(0, 0), RECTANGLE, .5));
	objects.push_back(new Polygon(Vec2(10, -8), RECTANGLE, .5));
	objects.push_back(new Polygon(Vec2(-4, -12), RECTANGLE, .5));
	objects.push_back(new Polygon(Vec2(-10, -2), RECTANGLE, .5));
	objects.push_back(new Polygon(Vec2(-20, -10), RECTANGLE, .5));
	for (int i = 1; i < 6; i++) {
		objects[i]->inverseMass = 0;
		objects[i]->collider->SetInvMass(0);
	}

	for (int i = 1; i < 6; i++) {
		objects.push_back(new Target(objects[i]->GetPos() + Vec2(0, 3), playerLauncher));
		objects[i + 5]->inverseMass = 0;
		objects[i + 5]->collider->SetInvMass(0);
	}

	objects.push_back(new Plane(Vec2(0, 1), 20, 1));
	for (PhysicsObject* object : objects) {
		std::cout << object->GUID << '\n';
	}

}

void PhysicsSim::Update(float deltaTime)
{

	playerLauncher->Tick(deltaTime);
	std::vector<CollisionInfo> allCollisions;
	std::vector<PhysicsObject*> objectsToClear;
	for (int w = 0; w < 1; w++) {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				CollisionInfo check = drCollision.DetectCollision(objects[i]->collider, objects[j]->collider);
				if (check.collided) {
					allCollisions.push_back(check);
				}
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
		if (c->markedForDeletion) {
			objectsToClear.push_back(c);
		}
	}

	for (PhysicsObject* c : objectsToClear) {
		if (std::find(objects.begin(), objects.end(), c) != objects.end()) {
			objects.erase(std::find(objects.begin(), objects.end(), c));
		}
	}


}

void PhysicsSim::OnLeftClick()
{
	//playerLauncher->Fire();
}


