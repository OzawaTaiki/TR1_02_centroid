#include "Box.h"
#include <Novice.h>
#include <imgui.h>
#include <func.h>
#define _USE_MATH_DEFINES
#include <cmath>

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

	rotate = float(-1.0f / 4.0f * M_PI);
	mass = 1.0f;

	verties[0] = {
		.constTransform = {-size.x / 2.0f,-size.y / 2.0f},
		.transform = {-size.x / 2.0f,-size.y / 2.0f},
		.weight = mass / 4.0f
	};
	verties[1] = {
		.constTransform = {size.x / 2.0f,-size.y / 2.0f },
		.transform = {size.x / 2.0f,-size.y / 2.0f },
		.weight = mass / 4.0f
	};
	verties[2] = {
		.constTransform = { -size.x / 2.0f,size.y / 2.0f},
		.transform = { -size.x / 2.0f,size.y / 2.0f},
		.weight = mass / 4.0f
	};
	verties[3] = {
		.constTransform = {size.x / 2.0f,size.y / 2.0f},
		.transform = {size.x / 2.0f,size.y / 2.0f},
		.weight = mass / 4.0f
	};

	CalculateCentroid();
}

void Box::Update()
{
	collisionDir = { 0,0 };


	Gravity();
	Rotate();

	ImGui::Begin("Box");
	ImGui::DragFloat2("position", &pos.x, 0.1f);
	ImGui::DragFloat2("Velocity", &velocity.x, 0.01f);
	ImGui::SliderAngle("angle", &rotate);
	if (ImGui::Button("angleReset"))
		rotate = -1.0f / 4.0f * (float)M_PI;
	ImGui::Text("vertex\n 0_x: %+.2f,\t0_y: %+.2f\n 1_x: %+.2f,\t1_y: %+.2f\n 2_x: %+.2f,\t2_y: %+.2f\n 3_x: %+.2f,\t3_y: %+.2f\n",
				verties[0].transform.x, verties[0].transform.y,
				verties[1].transform.x, verties[1].transform.y,
				verties[2].transform.x, verties[2].transform.y,
				verties[3].transform.x, verties[3].transform.y);
	ImGui::End();

	pos.x += velocity.x;
	pos.y += velocity.y;
}

void Box::Draw()
{
	Novice::DrawBox(static_cast<int>(pos.x - size.x / 2.0f), static_cast<int>(pos.y - size.x / 2.0f),
					static_cast<int>(size.x - 1), static_cast<int>(size.y - 1), 0.0f, BLACK, kFillModeWireFrame);


	Novice::DrawQuad(static_cast<int>(pos.x + verties[0].transform.x), static_cast<int>(pos.y + verties[0].transform.y),
					 static_cast<int>(pos.x + verties[1].transform.x), static_cast<int>(pos.y + verties[1].transform.y),
					 static_cast<int>(pos.x + verties[2].transform.x), static_cast<int>(pos.y + verties[2].transform.y),
					 static_cast<int>(pos.x + verties[3].transform.x), static_cast<int>(pos.y + verties[3].transform.y),
					 0, 0, 1, 1, rectGH, color);

	//Novice::DrawEllipse(static_cast<int>(pos.x), static_cast<int>(pos.y), 5, 5, 0, WHITE, kFillModeSolid);
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

void Box::GetVertiesTransform(Vector2 _verties[])
{
	_verties[0] = verties[0].transform;
	_verties[1] = verties[1].transform;
	_verties[2] = verties[2].transform;
	_verties[3] = verties[3].transform;
}

void Box::Rotate()
{
	for (Verties& vertex : verties)
	{
		vertex.transform = RotateVector(vertex.constTransform, rotate);
	}
	SortVertexArray();
}

void Box::CalculateCentroid()
{
	Vector2 sum = { 0,0 };
	float sumWeight = 0;
	int count = 0;

	for (Verties& v : verties)
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

void Box::SortVertexArray()
{
	// yがちいさいのとおおきいのを０と３へ
	// ほかでｘが小さいのと大きいのを１と２へ

	int yupIndex[2] = { 0 };
	int ydownIndex[2] = { 0 };
	int count = 0;
	Verties	copyVerties[4];
	memcpy(copyVerties, verties, sizeof(Verties) * 4);
	bool clear[4] = { false };//配列格納フラグ

	for (int i = 0; i < 4; i++)
	{
		//頂点が上にあるインデックスを把握
		if (copyVerties[i].transform.y < 0)
			yupIndex[count++] = i;
	}

	int index = -1;
	if ((int)copyVerties[yupIndex[0]].transform.y < (int)copyVerties[yupIndex[1]].transform.y)
	{
		index = yupIndex[0];
	}
	else if ((int)copyVerties[yupIndex[0]].transform.y > (int)copyVerties[yupIndex[1]].transform.y)
	{
		index = yupIndex[1];
	}

	if (index != -1)
	{
		//２つを比べて上にある頂点を配列の0へ格納
		verties[0] = copyVerties[index];
		clear[0] = true;

		index = index == yupIndex[0] ? yupIndex[1] : yupIndex[0];

		//下にあるものが左にあるなら２へ格納
		if ((int)copyVerties[index].transform.x < 0)
		{
			verties[2] = copyVerties[index];
			clear[2] = true;
		}
		//そうじゃなかった１へ
		else
		{
			verties[1] = copyVerties[index];
			clear[1] = true;
		}


	}

	count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (!clear[i])
		{
			//頂点が上にあるインデックスを把握
			if ((int)copyVerties[i].transform.y > 0)
				ydownIndex[count++] = i;
		}
	}

	index = -1;
	//大きいほうをindexへ
	if ((int)copyVerties[ydownIndex[0]].transform.y > (int)copyVerties[ydownIndex[1]].transform.y)
	{
		index = ydownIndex[0];
	}
	else if ((int)copyVerties[ydownIndex[0]].transform.y < (int)copyVerties[ydownIndex[1]].transform.y)
	{
		index = ydownIndex[1];
	}

	if (index != -1)
	{
		//２つを比べて下にある頂点を配列の0へ格納
		verties[3] = copyVerties[index];
		clear[3] = true;

		index = index == ydownIndex[0] ? ydownIndex[1] : ydownIndex[0];

		//上にあるものが左にあるなら２へ格納
		if ((int)copyVerties[index].transform.x < 0)
		{
			verties[2] = copyVerties[index];
			clear[2] = true;
		}
		//そうじゃなかった１へ
		else
		{
			verties[1] = copyVerties[index];
			clear[1] = true;
		}
	}

	// indexが-1じゃないということは回転している
	// フラグはすべてtrueになっているはず
	if (index != -1)
		return;


	// 回転しているけど０radと等しいとき
	// フラグがすべてfalseのはず
	for (int i = 0; i < 4; i++)
	{
		if (clear[i])
			continue;

		if (i == 3)
		{
			//assert(false);
		}
	}

}
