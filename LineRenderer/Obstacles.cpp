#include "Obstacles.h"
#include "LineRenderer.h"
void Bumper::Update(float dt)
{
	if (framesActive == 6) {
		bounceDrawSize = 1.2;
		drawBounce = false;
	}
	else {
		if (framesActive % 4 == 0) {
			bounceDrawSize += .2;

		}
		framesActive++;
	}
}
void Bumper::CollisionEvent(PhysicsObject* other)
{
	if (other->GetType() != STATIC) {
		Vec2 a = this->GetPos();
		Vec2 b = other->GetPos();

		Vec2 dir = (b - a).Normalise();
		thisBounceDir = dir;
		other->ApplyImpulse(dir * strength);
		this->drawBounce = true;
		framesActive = 0;
		bounceDrawSize = 1.2;
	}
}

void Bumper::Draw(LineRenderer* lines) const
{
	Circle::Draw(lines);

	if (drawBounce) {
		lines->DrawLineWithArrow((position - thisBounceDir * GetRadius()), (position + thisBounceDir * 1.5));
		lines->DrawCircle(position, bounceDrawSize, Colour::BLUE);
	}
}

void Blade::CollisionEvent(PhysicsObject* other)
{
	if (other->GetType() == DYNAMIC) {
		other->markedForDeletion = true;
	}
}

void Blade::Draw(LineRenderer* lines) const
{
	lines->SetColour(Colour::RED);
	Polygon::Draw(lines);
	lines->SetColour(Colour::WHITE);
}

BladeSpinners::BladeSpinners(Vec2 pos, int count, float rotSpeed, std::vector<PhysicsObject*>& queue)
{
	position = pos;
	Vec2 dispPos = Vec2(0, 1);
	rotationSpeed = rotSpeed;
	float fullRot = PI * 2;
	float rotInc = fullRot / count;
	for (int i = 0; i < count; i++) {
		Vec2 thisDisp = dispPos.GetRotatedBy(rotInc * i);
		//thisDisp.Normalise();
		thisDisp *= 2;
		Blade* b = new Blade(pos + thisDisp);
		b->inverseMass = 0;
		b->inverseMomentOfInertia = 0;
		b->orientation = (rotInc * i);
		queue.push_back(b);
		blades.push_back(b);
	}
}

BladeSpinners::~BladeSpinners()
{
	Unload();
}


void BladeSpinners::Update(float dt)
{
	for (Blade* b : blades) {
		b->RotateAbout(rotationSpeed * dt, position);
	}
}

void BladeSpinners::Draw(LineRenderer* lines) const
{
	lines->DrawCircle(position, .5);
}

void BladeSpinners::Unload()
{
	for (Blade* b : blades) {
		b->markedForDeletion = true;
	}
}

SpinBlock::SpinBlock(Vec2 pos, std::vector<Vec2> vertices, Vec2 dispFromOrigin)
	: Polygon(pos, vertices, 1)
{
	constraint = new HingeJoint(pos - dispFromOrigin);
	//this->joint = new HingeJoint(pos - Vec2(1, 0));
}

void SpinBlock::Update(float dt)
{
	constraint->Constrain(this);
	Polygon::Update(dt);
}

void SpinBlock::Draw(LineRenderer* lines) const
{
	Polygon::Draw(lines);
	lines->DrawCircle(constraint->position, .5);
}

void Crate::Update(float dt)
{
	//constraint->Constrain(this);
	Polygon::Update(dt);
}

void Crate::CollisionEvent(PhysicsObject* other)
{
	if (other->GetType() == STATIC) {
		this->markedForDeletion = true;
	}
}

