#include "Target.h"
#include "Launcher.h"
#include "LineRenderer.h"
#include "Launcher.h"
void Target::CollisionEvent(PhysicsObject* other)
{
	Bullet* bull = dynamic_cast<Bullet*>(other);

	if (bull != nullptr) {
		bull->score+= bull->combo;
		bull->combo++;
		this->markedForDeletion = true;
	}
}

void Target::Draw(LineRenderer* lines) const
{
	lines->DrawCircle(position, GetRadius());
	lines->DrawCircle(position, GetRadius() * .8, Colour::RED);
	lines->DrawCircle(position, GetRadius() * .6, Colour::YELLOW);
}

void FrenzyTarget::CollisionEvent(PhysicsObject* other)
{
	Bullet* bull = dynamic_cast<Bullet*>(other);

	Vec2 force = Vec2(-1, 0);
	float rAmnt = PI;
	rAmnt /= spawnCount;
	force.RotateBy(-rAmnt);
	if (bull) {
		for (int i = 0; i < spawnCount; i++) {
			force.RotateBy(rAmnt);
			Bullet* b = new Bullet(spawnPos + force, bull->player);
			obQueue.push_back(b);
			b->useGravity = true;
			b->ApplyImpulse(force * 5);
		}
	}

	Target::CollisionEvent(other);
}

void FrenzyTarget::Draw(LineRenderer* lines) const
{
	lines->DrawCircle(position, GetRadius());
	lines->DrawCircle(position, GetRadius() * .8, Colour::BLUE);
	lines->DrawCircle(position, GetRadius() * .6, Colour::GREEN);
}
