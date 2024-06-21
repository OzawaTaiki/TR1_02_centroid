#include "Box.h"
#include<Novice.h>
#include <imgui.h>

Box::Box(int _mapchipiSize, uint32_t _color) : kMapchipSize(static_cast<float>(_mapchipiSize)), color(_color)
{
	rectGH = Novice::LoadTexture("white1x1.png");
}

void Box::Initialize(const Vector2& _pos)
{
	pos = _pos;

	size = { static_cast<float>(kMapchipSize),static_cast<float> (kMapchipSize) };
	velocity = { 0.0f,0.0f };
	accelelation = { 0.0f,0.5f };

	collisionDir = { 0,0 };

	rotate = 0.0f;
	mass = 1.0f;

	verties[0] = {
		.transform = {-size.x / 2.0f,-size.y / 2.0f},
		.weight = mass / 4.0f
	};
	verties[1] = {
		.transform = {size.x / 2.0f,-size.y / 2.0f },
		.weight = mass / 4.0f
	};
	verties[2] = {
		.transform = { -size.x / 2.0f,size.y / 2.0f},
		.weight = mass / 4.0f
	};
	verties[3] = {
		.transform = {size.x / 2.0f,size.y / 2.0f},
		.weight = mass / 4.0f
	};

	CalculateCentroid();
}

void Box::Update()
{
	collisionDir = { 0,0 };

	Gravity();

	ImGui::Begin("Box");
	ImGui::DragFloat2("position", &pos.x, 0.1f);
	ImGui::DragFloat2("Velocity", &velocity.x, 0.01f);
	ImGui::End();

	pos.x += velocity.x;
	pos.y += velocity.y;
}

void Box::Draw()
{
	Novice::DrawQuad(static_cast<int>(pos.x + verties[0].transform.x), static_cast<int>(pos.y + verties[0].transform.y),
					 static_cast<int>(pos.x + verties[1].transform.x), static_cast<int>(pos.y + verties[1].transform.y),
					 static_cast<int>(pos.x + verties[2].transform.x), static_cast<int>(pos.y + verties[2].transform.y),
					 static_cast<int>(pos.x + verties[3].transform.x), static_cast<int>(pos.y + verties[3].transform.y),
					 0, 0, 1, 1, rectGH, color);

	Novice::DrawEllipse(static_cast<int>(pos.x), static_cast<int>(pos.y), 5, 5, 0, WHITE, kFillModeSolid);
}

void Box::SetCollisionDir(const Vector2& _col)
{
	collisionDir = _col;
}

void Box::CollisonWithField()
{
	Vector2 nextPos = pos;
	nextPos.x += velocity.x;
	nextPos.y += velocity.y;

	if (collisionDir.x != 0)
	{
		//左がfieldに当たった
		if (collisionDir.x < 0)
		{
			pos.x = static_cast<int>((nextPos.x - size.x / 2.0f) / kMapchipSize + 1) * kMapchipSize + size.x / 2.0f;
		}
		else
		{
			pos.x = static_cast<int>((nextPos.x + size.x / 2.0f + 0.5f) / kMapchipSize) * kMapchipSize - size.x / 2.0f;
		}
		collisionDir.x = 0;
	}
	if (collisionDir.y != 0)
	{
		//上がfieldに当たった
		if (collisionDir.y < 0)
		{
			pos.y = static_cast<int>((nextPos.y - size.y / 2.0f) / kMapchipSize + 1) * kMapchipSize + size.y / 2.0f;
		}
		else
		{
			pos.y = static_cast<int>(int(nextPos.y + size.y / 2.0f + 0.5f) / kMapchipSize + 0.25f) * kMapchipSize - size.y / 2.0f;
			velocity.y = 0;
		}
		collisionDir.y = 0;

	}

}

void Box::CalculateCentroid()
{
	Vector2 sum = { 0,0 };
	float sumWeight = 0;
	int count = 0;

	for (Verties v : verties)
	{
		count++;
		sum.x += v.weight * v.transform.x;
		sum.y += v.weight * v.transform.y;
		sumWeight += v.weight;
	}

	centroid.x = sum.x / static_cast<float>(count);
	centroid.y = sum.y / static_cast<float>(count);
	mass = sumWeight;

	return;

}

void Box::Gravity()
{
	const float kMaxVelocity = 15.0f;
	velocity.y += accelelation.y;
	if (velocity.y > kMaxVelocity)
		velocity.y = kMaxVelocity;
}
