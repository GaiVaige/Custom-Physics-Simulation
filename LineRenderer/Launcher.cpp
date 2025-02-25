#include "Launcher.h"
#include "LineRenderer.h"
#include "ApplicationHarness.h"
#include <iostream>
Launcher::Launcher(Vec2 pos, Vec2 bDisp)
	: Polygon(pos, launcherVerts, 0, STATIC)
{
	for (Vec2& v : barrelverts) {
		v += bDisp;
	}
	barrel = new Barrel(pos);
	barrelPos = bDisp;
}

Launcher::~Launcher()
{
	delete barrel;
}

void Launcher::Tick(float dt)
{
	currentTime += dt;
	rulerPositions.clear();
	float rotAmnt = ApplicationHarness::GetInputAxis(Key::RightArrow, Key::LeftArrow);

	float strAmnt = ApplicationHarness::GetInputAxis(Key::DownArrow, Key::UpArrow);

	if(ApplicationHarness::IsKeyDown(Key::Space) && currentTime > fireTimer) {
		Fire();
		currentTime = 0;
	}

	AdjustStrength(strAmnt * dt);
	AdjustAngle(rotAmnt * dt);

	Vec2 initialPos = position + barrelPos.GetRotatedBy(barrel->GetOrientation()) + barrel->up * .8;
	Bullet* b = new Bullet(initialPos);
	b->useGravity = true;
	b->ApplyImpulse(barrel->up * fireStrength);

	for (int i = 0; i < 2 * fireStrength; i++) {
		for (int i = 0; i < 5; i++) {
			b->Update(dt);
		}
		rulerPositions.push_back(b->GetPos());
	}
	delete b;
}

void Launcher::Fire()
{
	if (fireStrength != 0) {
		Bullet* b = new Bullet(position + barrelPos.GetRotatedBy(barrel->GetOrientation()) + barrel->up * .8);
		sceneObjects->push_back(b);
		b->useGravity = true;
		b->ApplyImpulse(barrel->up * fireStrength);
	}

}

void Launcher::AdjustStrength(float amnt)
{
	fireStrength += amnt * 3;
	if (fireStrength > 10) fireStrength = 10;
	if (fireStrength < 0) fireStrength = 0;
}

void Launcher::AdjustAngle(float amnt)
{
	if (RadToDeg(bOrient + amnt) > 56) amnt = 0;
	if (RadToDeg(bOrient + amnt) < -56) amnt = 0;

	barrel->Rotate(amnt);
	bOrient = barrel->GetOrientation();
}

void Launcher::Draw(LineRenderer* lines) const
{
	Polygon::Draw(lines);
	std::string degText = std::to_string(RadToDeg(barrel->GetOrientation()));
	int size = degText.find('.');
	degText.resize(size + 3);
	lines->DrawText("Angle: " + degText, Vec2(-39.3, 20), 1);
	std::string fireText = std::to_string(fireStrength);
	int fsize = fireText.find('.');
	fireText.resize(fsize + 2);
	lines->DrawText("Fire Strength: " + fireText + "/10", Vec2(-39.3, 18), 1);
	lines->DrawText("Score: " + std::to_string(score), Vec2(-39.3, 16), 1);
	barrel->Draw(lines);

	lines->AddPointToLine(position + barrelPos.GetRotatedBy(barrel->GetOrientation()) + barrel->up * .8);
	for (const Vec2& v : rulerPositions) {
		lines->AddPointToLine(v);
	}
	lines->FinishLineStrip();

}

void Bullet::Draw(LineRenderer* lines) const
{
	Circle::Draw(lines);
}

void Bullet::Notify(PhysicsObject* other)
{
	if (other->GetType() == STATIC) {
		markedForDeletion = true;
	}
}
