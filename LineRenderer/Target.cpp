#include "Target.h"
#include "Launcher.h"
#include "LineRenderer.h"
#include "Launcher.h"
void Target::Notify(PhysicsObject* other)
{
	Bullet* bull = dynamic_cast<Bullet*>(other);

	if (bull != nullptr) {
		player->score++;
		this->markedForDeletion = true;
	}
}

void Target::Draw(LineRenderer* lines) const
{
	lines->DrawCircle(position, GetRadius());
	lines->DrawCircle(position, GetRadius() * .8, Colour::RED);
	lines->DrawCircle(position, GetRadius() * .6, Colour::YELLOW);
}
