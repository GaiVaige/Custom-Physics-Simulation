#pragma once
#include "Polygon.h"
#include "Circle.h"

class Launcher;
struct Bullet : public Circle {
	Bullet(Vec2 pos, Launcher* player = nullptr) : Circle(pos, .6, 1), player(player) {};

	Launcher* player;
	int score = 0;
	int combo = 1;
	void Draw(LineRenderer* lines) const override;
	void CollisionEvent(PhysicsObject* other) override;

};

static std::vector<Vec2> barrelverts{
	{-.1, -.5},
	{-.1, .5},
	{.1, .5},
	{.1, -.5}
};

struct Barrel : public Polygon {
	Barrel(Vec2 pos) : Polygon(pos, barrelverts, 0) {};
};

static std::vector<Vec2> launcherVerts{
	{-1, 0},
	{-.5, 1},
	{.5, 1},
	{1, 0}
};

class Launcher : public Polygon {

public:
	Launcher(Vec2 pos, Vec2 bDisp);
	~Launcher();

	Barrel* barrel;
	std::vector<PhysicsObject*>* sceneObjects;


	Vec2 barrelPos;
	float fireStrength;
	float bOrient;


	void Update(float dt) override;
	void Fire();
	void AdjustStrength(float amnt);
	void AdjustAngle(float amnt);

	void Draw(LineRenderer* lines) const override;

	std::vector<Vec2> rulerPositions;
	float fireTimer = .8f;
	float currentTime = 0;
	int score = 0;
private:

};