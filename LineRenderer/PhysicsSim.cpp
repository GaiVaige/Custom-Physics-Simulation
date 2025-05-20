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

std::vector<Vec2> ShapeA = {
	Vec2(8.73, 1.9),
	Vec2(10.85, -3.6),
	Vec2(-10.1, 2.6),
	Vec2(-9.58, 4.57)
};

std::vector<Vec2> ShapeB = {
	Vec2(-10.937500, -9.812500),
	Vec2(4.250000, 17.875000),
	Vec2(6.687500, -8.062500),
};

std::vector<Vec2> ShapeC = {
	Vec2(-9.857143, -2.026785),
	Vec2(-9.169643, 5.098215),
	Vec2(-0.857143, 9.785715),
	Vec2(6.455357, 5.223215),
	Vec2(10.267857, -3.276785),
	Vec2(6.892857, -8.026785),
	Vec2(-3.732143, -6.776785),
};

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

	//Object creation
	Polygon* p = new Polygon(Vec2(-25, 7.5), ShapeA, 1);
	p->inverseMass = 0;
	p->inverseMomentOfInertia = 0;
	objects.push_back(p);
	Polygon* p2 = new Polygon(Vec2(28.687500, 11.187500), ShapeB, 1);
	p2->inverseMass = 0;
	p2->inverseMomentOfInertia = 0;
	objects.push_back(p2);
	Polygon* p3 = new Polygon(Vec2(-22.142857, 26.026785), ShapeC, 1);
	p3->inverseMass = 0;
	p3->inverseMomentOfInertia = 0;
	objects.push_back(p3);
	
	objects.push_back(new Target(Vec2(-6.125000, 7.687500)));
	objects.push_back(new Target(Vec2(10.375000, 9.937500)));
	objects.push_back(new Target(Vec2(-15.375000, 11.7500001)));
	objects.push_back(new Target(Vec2(19.750000, 12.687498)));
	objects.push_back(new FrenzyTarget(Vec2(23.437500, 23.687500), 10, Vec2(50, 0), objectQueue));
	objects.push_back(new Target(Vec2(17.312500, 21.062500)));
	objects.push_back(new Target(Vec2(20.875000, 32.125000)));
	objects.push_back(new Target(Vec2(- 15.750000, 35.375000)));
	objects.push_back(new Target(Vec2(- 9.187500, 30.125000)));
	objects.push_back(new FrenzyTarget(Vec2(- 9.000000, 19.687502), 10, Vec2(50, 0), objectQueue));
	objects.push_back(new Target(Vec2(- 22.125000, 13.874998)));
	objects.push_back(new Target(Vec2(- 31.000000, 14.437500)));
	objects.push_back(new Target(Vec2(8.437500, 35.562500)));
	
	objects.push_back(new Bumper(Vec2(-8.500000, 14.187500), 5));
	objects.push_back(new Bumper(Vec2(7.687500, 8.062500), 5));
	objects.push_back(new Bumper(Vec2(16.187500, 18.562500), 5));
	objects.push_back(new Bumper(Vec2(- 7.687500, 24.562500), 5));
	objects.push_back(new Bumper(Vec2(16.687500, 26.250000), 5));
	objects.push_back(new Bumper(Vec2(13.750000, 34.062500), 5));
	
	objects.push_back(new BladeSpinners(Vec2(-7.312500, 38.437500), 5, 1, objectQueue));
	objects.push_back(new BladeSpinners(Vec2(3.044551, 25), 5, 1, objectQueue));
	
	objects.push_back(new SpinBlock(Vec2(-5.580449, 26.437500), RECTANGLE, Vec2(-2, 0)));
	objects.push_back(new SpinBlock(Vec2(9.312500, 18.499998), RECTANGLE, Vec2(2, 0)));
	objects.push_back(new SpinBlock(Vec2(- 2.687500, 12.687498), RECTANGLE, Vec2(2, 0)));



	for (PhysicsObject* c : objectQueue) {
		objects.push_back(c);
	}
	objectQueue.clear();
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

	case Key::Escape:
		ApplicationHarness::Quit();
	default:
		break;
	}
}



