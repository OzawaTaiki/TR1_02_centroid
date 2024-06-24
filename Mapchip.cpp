#include "Mapchip.h"
#include "CSVLoader.h"
#include <Novice.h>

void Mapchip::Initialize()
{
	CSVLoader::LoadCSV(path[0]);
	map = *CSVLoader::GetDataPointer();

	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 2)
			{
				Vector2 boxPos;
				boxPos.x = static_cast<float>(x) * kMapchipSize;
				boxPos.y = static_cast<float>(y) * kMapchipSize;
				box.push_back(new Box(kMapchipSize, 0xaa0000ff));
				box.back()->Initialize(boxPos);
				map[y][x] = 0;
			}
		}
	}
}

void Mapchip::Update()
{
	for (Box* nBox : box)
	{
		nBox->Update();
	}

	CheckCollision();
}

void Mapchip::Draw()
{
	for (Box* nBox : box)
	{
		nBox->Draw();
	}

	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 1)
				Novice::DrawBox(x * kMapchipSize, y * kMapchipSize, kMapchipSize - 1, kMapchipSize - 1, 0, WHITE, kFillModeSolid);
		}
	}
}

bool Mapchip::CheckCollision()
{
	Vector2 position = box[0]->GetPosition();
	Vector2 halfSize = box[0]->GetSize();	//半分のサイズ
	halfSize.x /= 2.0f;
	halfSize.y /= 2.0f;
	Vector2 velocity = box[0]->GetVelocity();

	Vector2 Verties[4];
	box[0]->GetVertiesTransform(Verties);
	Vector2 hit{ 0,0 };

	if (velocity.x != 0)
	{
		//TODO: 頂点でしか判定取れいない
		// 辺でも判定とれるように
		if ((map[int((position.y + Verties[0].y) / kMapchipSize)][int((position.x + Verties[0].x + velocity.x) / kMapchipSize)] != 0 ||///左上
			map[int((position.y + Verties[2].y - 1) / kMapchipSize)][int((position.x + Verties[2].x + velocity.x) / kMapchipSize)] != 0) &&///左下
			velocity.x < 0)///左に動いている
		{
			hit.x = -1;
		}
		else if ((map[int((position.y + Verties[1].y) / kMapchipSize)][int((position.x + Verties[1].x - 1 + velocity.x) / kMapchipSize)] != 0 ||///右上
				 map[int((position.y + Verties[3].y - 1) / kMapchipSize)][int((position.x + Verties[3].x - 1 + velocity.x) / kMapchipSize)] != 0) &&///右下
				 velocity.x > 0)///右に動いている
		{
			hit.x = 1;
		}
		box[0]->SetCollisionDir(hit);
		box[0]->CollisonWithField();
	}
	if (velocity.y != 0)
	{
		if ((map[int((position.y + Verties[0].y + velocity.y) / kMapchipSize)][int((position.x + Verties[0].x) / kMapchipSize)] != 0 ||///左上
			map[int((position.y + Verties[1].y + velocity.y) / kMapchipSize)][int((position.x + Verties[1].x - 1) / kMapchipSize)] != 0) &&///右上
			velocity.y < 0)///上に動いている
		{
			hit.y = -1;
		}
		else if ((map[int((position.y + Verties[2].y + velocity.y - 1) / kMapchipSize)][int((position.x + Verties[2].x) / kMapchipSize)] != 0 ||///左下
				 map[int((position.y + Verties[3].y + velocity.y - 1) / kMapchipSize)][int((position.x + Verties[3].x - 1) / kMapchipSize)] != 0) &&///右下
				 velocity.y > 0)///下に動いている
		{
			hit.y = 1;
		}
		box[0]->SetCollisionDir(hit);
		box[0]->CollisonWithField();
	}


	return false;
}
