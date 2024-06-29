#include "Mapchip.h"
#include "CSVLoader.h"
#include <Novice.h>
#include "func.h"
#include <imgui.h>

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
				box.push_back(new Box(kMapchipSize, 0xaa0000a0));
				box.back()->Initialize(boxPos);
				map[y][x] = 0;
			}
		}
	}

	ISDrawDrid = false;
	gridColor = 0x80808080;
	color[0] = 0.5f;
	color[1] = 0.5f;
	color[2] = 0.5f;
	color[3] = 0.5f;
}

void Mapchip::Update()
{
	for (Box* nBox : box)
	{
		nBox->Update();
	}

	CheckCollision();

	ShowImGui();
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
		if (ISDrawDrid)
			DrawGrid();
	}
}

void Mapchip::ShowImGui()
{
	ImGui::Checkbox("Grid", &ISDrawDrid);
	ImGui::ColorEdit4("color", &color[0]);

	uint8_t r = static_cast<uint8_t>(color[0] * 255.0f);
	uint8_t g = static_cast<uint8_t>(color[1] * 255.0f);
	uint8_t b = static_cast<uint8_t>(color[2] * 255.0f);
	uint8_t a = static_cast<uint8_t>(color[3] * 255.0f);

	gridColor = (a << 24) | (b << 16) | (g << 8) | r;
}

void Mapchip::DrawGrid()
{
	for (int y = 0; y < map.size(); y++)
	{
		Novice::DrawLine(0, y * kMapchipSize, 1280, y * kMapchipSize, gridColor);
	}
	for (int x = 0; x < map[0].size(); x++)
	{
		Novice::DrawLine(x * kMapchipSize, 0, x * kMapchipSize, 720, gridColor);

	}
}

bool Mapchip::CheckCollision()
{
	for (Box* nBox : box)
	{
		Vector2 position = nBox->GetPosition();
		Vector2 halfSize = nBox->GetSize();	//半分のサイズ
		halfSize.x /= 2.0f;
		halfSize.y /= 2.0f;
		Vector2 velocity = nBox->GetVelocity();
		Vector2 nextPos = position + velocity;

		Vector2 Verties[4];
		nBox->GetVertiesTransform(Verties);
		Vector2 hitDirection{ 0,0 };
		Vector2 hit;

		for (int i = 0; i < 4; i++)
		{
			if (map[int((nextPos.y + Verties[i].y) / kMapchipSize)][int((nextPos.x + Verties[i].x) / kMapchipSize)] != 0)
			{
				nBox->RegistHitPos(nextPos + Verties[i]);
			}
		}

		if (velocity.y != 0)
		{
			if ((map[int((position.y + Verties[0].y + velocity.y) / kMapchipSize)][int((position.x + Verties[0].x) / kMapchipSize)] != 0 ||///左上
				map[int((position.y + Verties[1].y + velocity.y) / kMapchipSize)][int((position.x + Verties[1].x - 1) / kMapchipSize)] != 0) &&///右上
				velocity.y < 0)///上に動いている
			{
				hitDirection.y = -1;
			}
			else if ((map[int((position.y + Verties[2].y + velocity.y - 1) / kMapchipSize)][int((position.x + Verties[2].x) / kMapchipSize)] != 0 ||///左下
					 map[int((position.y + Verties[3].y + velocity.y - 1) / kMapchipSize)][int((position.x + Verties[3].x - 1) / kMapchipSize)] != 0) &&///右下
					 velocity.y > 0)///下に動いている
			{
				hitDirection.y = 1;
			}
			nBox->SetCollisionDir(hitDirection);
			nBox->CollisonWithField();
		}

		position = nBox->GetPosition();
		velocity = nBox->GetVelocity();
		if (velocity.x != 0)
		{
			if ((map[int((position.y + Verties[0].y) / kMapchipSize)][int((position.x + Verties[0].x + velocity.x) / kMapchipSize)] != 0 ||///左上
				map[int((position.y + Verties[2].y - 1) / kMapchipSize)][int((position.x + Verties[2].x + velocity.x) / kMapchipSize)] != 0) &&///左下
				velocity.x < 0)///左に動いている
			{
				hitDirection.x = -1;
			}
			else if ((map[int((position.y + Verties[1].y) / kMapchipSize)][int((position.x + Verties[1].x - 1 + velocity.x) / kMapchipSize)] != 0 ||///右上
					 map[int((position.y + Verties[3].y - 1) / kMapchipSize)][int((position.x + Verties[3].x - 1 + velocity.x) / kMapchipSize)] != 0) &&///右下
					 velocity.x > 0)///右に動いている
			{
				hitDirection.x = 1;
			}
			nBox->SetCollisionDir(hitDirection);
			nBox->CollisonWithField();
		}

		position = nBox->GetPosition();

		//どこの頂点も当たっていなかったとき
		//辺の判定をとりたい↓
		int xMax = int((nextPos.x + Verties[1].x) / kMapchipSize);
		int xMin = int((nextPos.x + Verties[2].x) / kMapchipSize);

		int yMin = int((nextPos.y + Verties[0].y) / kMapchipSize);
		int yMax = int((nextPos.y + Verties[3].y) / kMapchipSize);

		DrawAABB((int)(xMin * kMapchipSize), (int)(yMin * kMapchipSize), (int)(xMax * kMapchipSize + 64), (int)(yMax * kMapchipSize + 64), 0xa0000080);

		Vector2 crossPos;
		for (int i = xMin; i < xMax; i++)
		{
			if (map[yMin][i] != map[yMin][i + 1])
			{
				hit = { float(i + 1) * kMapchipSize ,(float)yMin * kMapchipSize };
			}

			if (map[yMax][i] != map[yMax][i + 1])
			{
				//下の頂点より左
				if (Verties[3].x + position.x > float(i + 1) * kMapchipSize)
				{
					hit = { float(i + 1) * kMapchipSize ,(float)yMax * kMapchipSize };
					//23ベクトルとマップチップの交点を取る
					Vector2 max = { hit.x,hit.y + kMapchipSize };
					crossPos = getCrossPos(position + Verties[2], position + Verties[3], hit, max);
					Novice::DrawEllipse(int(crossPos.x), int(crossPos.y), 3, 3, 0, 255, kFillModeSolid);

					//交点がブロックのとき
					if (map[int(crossPos.y / kMapchipSize)][int((crossPos.x - 1) / kMapchipSize)] != 0)
					{
						//box中心と交点の差分ベクトルを求める
						Vector2 boxCenterToCross = crossPos - position;
						Vector2 afterPos = crossPos;
						afterPos.x = int(crossPos.x / kMapchipSize) * kMapchipSize - boxCenterToCross.x;
						afterPos.y = int(crossPos.y / kMapchipSize) * kMapchipSize - boxCenterToCross.y;
						nBox->SetPosition(afterPos);

						//交点yはpos-boxCenterToCross
						crossPos.y = afterPos.y + boxCenterToCross.y;
						nBox->RegistHitPos(crossPos);


					}
				}
				else if (Verties[3].x + position.x < float(i + 1) * kMapchipSize)
				{
					hit = { float(i + 1) * kMapchipSize ,(float)yMax * kMapchipSize };

					//23ベクトルとマップチップの交点を取る
					Vector2 max = { hit.x,hit.y + kMapchipSize };
					//Novice::DrawEllipse(int(hit.x), int(hit.y), 10, 10, 0, 255, kFillModeSolid);
					crossPos = getCrossPos(position + Verties[1], position + Verties[3], hit, max);
					Novice::DrawEllipse(int(crossPos.x), int(crossPos.y), 3, 3, 0, 255, kFillModeSolid);

					//交点がブロックのとき
					if (map[int(crossPos.y / kMapchipSize)][int((crossPos.x + 1) / kMapchipSize)] != 0)
					{
						//box中心と交点の差分ベクトルを求める
						Vector2 boxCenterToCross = crossPos - position;
						Vector2 afterPos = crossPos;
						afterPos.x = int(crossPos.x / kMapchipSize) * kMapchipSize - boxCenterToCross.x;
						afterPos.y = int(crossPos.y / kMapchipSize) * kMapchipSize - boxCenterToCross.y;
						nBox->SetPosition(afterPos);

						//交点yはpos-boxCenterToCross
						crossPos.y = afterPos.y + boxCenterToCross.y;
						nBox->RegistHitPos(crossPos);

					}
				}
			}

		}

	}
	return false;

}
