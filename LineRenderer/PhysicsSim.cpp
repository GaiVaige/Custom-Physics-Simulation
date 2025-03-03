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
#include "Obstacles.h"
#pragma region ShapeVerts

std::vector<Vec2> BIGSQUARE = {
Vec2(-10, -10),
Vec2(-10, 10),
Vec2(10, 10),
Vec2(10, -10)
};

std::vector<Vec2> RECTANGLE = {
Vec2(-2, -.5f),
Vec2(-2, .5f),
Vec2(2, .5f),
Vec2(2, -.5f)
};

std::vector<Vec2> LONG_RECTANGLE = {
Vec2(-4, -.5f),
Vec2(-4, .5f),
Vec2(4, .5f),
Vec2(4, -.5f)
};
#pragma endregion



PhysicsSim::PhysicsSim()
{
	appInfo.appName = "Great Steal Caro";
	appInfo.horizontalResolution = 1280;
	appInfo.verticalResolution = 720;
	appInfo.camera.disable = true;
	cameraCentre = Vec2(0, 20);
	cameraHeight = 45;
	appInfo.grid.show = false;
	appInfo.grid.extent = 50;
}

PhysicsSim::~PhysicsSim()
{
	for (PhysicsObject* c : objects) {
		delete c;
	}

}
Launcher* playerLauncher;
void PhysicsSim::Initialise()
{
	playerLauncher = new Launcher(Vec2(0, 0), Vec2(0, 1.5));
	objects.push_back(playerLauncher);
	playerLauncher->sceneObjects = &objectQueue;

	objects.push_back(new Plane(Vec2(0, 1), 0, .2));
	objects.push_back(new Plane(Vec2(-1, 0), 50, .2));
	objects.push_back(new Plane(Vec2(1, 0), 50, .2));
	objects.push_back(new Plane(Vec2(0, -1), 51, .2));
	for (PhysicsObject* c : objectQueue) {
		objects.push_back(c);
	}
	for (PhysicsObject* object : objects) {
		std::cout << object->GUID << '\n';
	}

}

void PhysicsSim::Update(float deltaTime)
{
	for (PhysicsObject* c : objectQueue) {
		objects.push_back(c);
	}
	objectQueue.clear();
	std::vector<CollisionInfo> allCollisions;
	std::vector<PhysicsObject*> objectsToClear;
	for (int w = 0; w < 1; w++) {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				if (objects[i]->collider == nullptr || objects[j]->collider == nullptr) continue;
				CollisionInfo check = drCollision.DetectCollision(objects[i]->collider, objects[j]->collider);
				if (check.collided) {
					allCollisions.push_back(check);
				}
			}
		}
		for (CollisionInfo& collision : allCollisions) {
			drCollision.ResolveCollision(collision);
			//lines->DrawLineWithArrow(collision.contactPoint, collision.contactPoint + collision.normal);
		}
		allCollisions.clear();
	}
	for (PhysicsObject* c : objects) {
		c->Update(deltaTime);
		c->Draw(lines);
		if (c->markedForDeletion) {
			objectsToClear.push_back(c);
		}
	}


	for (PhysicsObject* c : objectsToClear) {
		if (std::find(objects.begin(), objects.end(), c) != objects.end()) {
			objects.erase(std::find(objects.begin(), objects.end(), c));
			delete c;
		}
	}
	for (Vec2 v : storedVerts) {
		Vec2 pos = Vec2(0, 0);
		for (Vec2& v : storedVerts) {
			lines->AddPointToLine(v, Colour::GREY);
			pos += v;
		}
		pos /= storedVerts.size();
		lines->FinishLineLoop();
		lines->DrawCircle(pos, 1);
	}


}

void PhysicsSim::OnLeftClick()
{
	storedVerts.push_back(cursorPos);
}

void PhysicsSim::OnRightClick()
{
	if (storedVerts.size() >= 3) {
		Vec2 pos = Vec2(0, 0);
		for (Vec2& v : storedVerts) {
			pos += v;
		}
		pos /= storedVerts.size();
		for (Vec2& v : storedVerts) {
			v -= pos;
		}
		float cwCalc = 0;
		for (int i = 0; i < storedVerts.size(); i++) {
			int j = i + 1;
			if (j == storedVerts.size()) j = 0;
			Vec2 a = storedVerts[i];
			Vec2 b = storedVerts[j];
			float add = (storedVerts[j].x - storedVerts[i].x) * (storedVerts[j].y + storedVerts[i].y);
			cwCalc += add;
		}
		//std::cout << cwCalc << '\n';
		if (cwCalc < 0) {
			std::reverse(storedVerts.begin(), storedVerts.end());
		}
		Polygon* p = new Polygon(pos, storedVerts, 1);
		p->inverseMass = 0;
		p->inverseMomentOfInertia = 0;

		//Validate if polygon is convex before scene placement
		bool signNegative = false;
		bool previousSign = signNegative;
		bool first = true;
		PolygonCollider* pc = static_cast<PolygonCollider*>(p->collider);
		for (int i = 0; i < p->GetVerts().size(); i++) {
			int j = i + 1;
			if (j == p->GetVerts().size()) j = 0;
			float f = PseudoCross(pc->GetAxis(i), pc->GetAxis(j));
			signNegative = (f < 0);
			if (first) {
				previousSign = signNegative;
				first = false;
			}
			if (signNegative != previousSign) {
				delete p;
				storedVerts.clear();
				return;
			}
			previousSign = signNegative;
		}
		objectQueue.push_back(p);

	}
	storedVerts.clear();

}

void PhysicsSim::OnKeyPress(Key key)
{

	switch (key) {
	case Key::One:
		objectQueue.push_back(new Target(cursorPos));
		break;
	case Key::Two:
		objectQueue.push_back(new BladeSpinners(cursorPos, 3, 1, objectQueue));
		break;
	case Key::Three:
		objectQueue.push_back(new Bumper(cursorPos, 5));
		break;
	case Key::Four:
		objectQueue.push_back(new FrenzyTarget(cursorPos, 10, Vec2(0, 50), objectQueue));
		break;
	case Key::Five:
		objectQueue.push_back(new SpinBlock(cursorPos, RECTANGLE, Vec2(-2, 0)));
		break;
	case Key::Six:
		objectQueue.push_back(new SpinBlock(cursorPos, RECTANGLE, Vec2(2, 0)));
		break;
	case Key::Seven:
		objectQueue.push_back(new Crate(cursorPos));
		break;
	case Key::R:
		for (int i = 5; i < objects.size(); i++) {
			objects[i]->markedForDeletion = true;
		}
		break;
	case Key::Z:
		if (objects.size() < 6) break;
		objects[objects.size() - 1]->markedForDeletion = true;
		break;
	default:
		break;
	}
}



