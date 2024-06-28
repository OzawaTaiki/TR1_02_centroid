#include "Box.h"
#include <Novice.h>
#include <imgui.h>
#include "func.h"
#include <cmath>
#include <string>
#include <numbers>

Box::Box(int _mapchipiSize, uint32_t _color) : kMapchipSize(static_cast<float>(_mapchipiSize)), color(_color)
{
	rectGH = Novice::LoadTexture("white1x1.png");
}

void Box::Initialize(const Vector2& _pos, const Vector2& _size)
{
	pos = _pos;

	if (_size.x == -1)
		size = { static_cast<float>(kMapchipSize),static_cast<float> (kMapchipSize) };
	else
		size = _size;
	velocity = { 0.0f,0.0f };
	accelelation = { 0.0f,0.5f };

	collisionDir = { 0,0 };

	angle = 0.0f;
	preAngle = 0.0f;
	angleVelocity = 0.0f;
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

	isGravity = true;

	CalculateCentroid();
}

void Box::Update()
{
	preAngle = angle;
	ShowImGui();
	collisionDir = { 0,0 };

	angle += angleVelocity;
	if (isGravity)
		Gravity();
	Rotate();
	CalculateCentroid();



	pos.x += velocity.x;
	pos.y += velocity.y;

	hitPosWithField.clear();

}

void Box::Draw()
{
	Novice::DrawBox(static_cast<int>(pos.x - size.x / 2.0f), static_cast<int>(pos.y - size.y / 2.0f),
					static_cast<int>(size.x - 1), static_cast<int>(size.y - 1), 0.0f, BLACK, kFillModeWireFrame);
	Novice::DrawBox(static_cast<int>(pos.x - size.y / 2.0f), static_cast<int>(pos.y - size.x / 2.0f),
					static_cast<int>(size.y - 1), static_cast<int>(size.x - 1), 0.0f, BLACK, kFillModeWireFrame);


	Novice::DrawQuad(static_cast<int>(pos.x + verties[0].transform.x), static_cast<int>(pos.y + verties[0].transform.y),
					 static_cast<int>(pos.x + verties[1].transform.x), static_cast<int>(pos.y + verties[1].transform.y),
					 static_cast<int>(pos.x + verties[2].transform.x), static_cast<int>(pos.y + verties[2].transform.y),
					 static_cast<int>(pos.x + verties[3].transform.x), static_cast<int>(pos.y + verties[3].transform.y),
					 0, 0, 1, 1, rectGH, color);

	Novice::DrawEllipse(static_cast<int>(pos.x + centroid.x), static_cast<int>(pos.y + centroid.y), 5, 5, 0, 0xaaaaaaaa, kFillModeSolid);
	Novice::DrawEllipse(static_cast<int>(pos.x), static_cast<int>(pos.y), 5, 5, 0, 0xaa, kFillModeSolid);

	for (const Vector2& p : hitPosWithField)
	{
		Novice::DrawEllipse(static_cast<int>(p.x), static_cast<int>(p.y), 5, 5, 0, 0xaa, kFillModeSolid);
	}


}

void Box::SetCollisionDir(const Vector2& _col)
{
	collisionDir = _col;
}

void Box::SetPosition(const Vector2& _pos)
{
	pos = _pos;
	velocity.y = 0;
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
			pos.x = static_cast<int>((nextPos.x + verties[2].transform.x) / kMapchipSize + 1) * kMapchipSize - verties[2].transform.x;
		}
		else
		{
			pos.x = static_cast<int>((nextPos.x + verties[1].transform.x + 0.5f) / kMapchipSize) * kMapchipSize - verties[1].transform.x;
		}
		collisionDir.x = 0;
	}
	if (collisionDir.y != 0)
	{
		//上がfieldに当たった
		if (collisionDir.y < -0.5f)
		{
			pos.y = static_cast<int>((nextPos.y + verties[0].transform.y) / kMapchipSize + 1) * kMapchipSize - verties[0].transform.y;
		}
		else if (collisionDir.y > 0.5f)
		{
			pos.y = static_cast<int>(int(nextPos.y + verties[3].transform.y + 0.5f) / kMapchipSize + 0.25f) * kMapchipSize - verties[3].transform.y;
		}
		velocity.y = 0;
		collisionDir.y = 0;
	}
}

void Box::RegistHitPos(Vector2 _hitPos)
{
	Vector2 regist = _hitPos;
	regist.y = int((_hitPos.y + 1) / kMapchipSize) * kMapchipSize;
	hitPosWithField.push_back(regist);
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
	if (angle >= 2.0f * (float)std::numbers::pi ||
		angle <= -2.0f * (float)std::numbers::pi)
	{
		angle = 0.0f;
	}

	if (angleVelocity == 0 && preAngle == angle)
		return;
	float rotateOfFrame = angle - preAngle;

	Matrix3x3 worldMat = MakeAffineMatrix({ 1.0f,1.0f }, angle, pos);
	Matrix3x3 worldMatInv = Inverse(worldMat);

	// p3を求める
	Vector2 rotateCenter = FindRotateCenter();
	Vector2 tp3 = Transform(rotateCenter, worldMatInv);
	Vector2 p3 = rotateCenter - pos;
	Vector2 ip3 = p3 * -1;
	Vector2 rp3 = RotateVector(ip3, rotateOfFrame);


	Novice::DrawEllipse(static_cast<int>(pos.x + p3.x), static_cast<int>(pos.y + p3.y), 5, 5, 0, 0xaaaaaaaa, kFillModeSolid);


	for (int i = 0; i < 4; i++)
	{
		verties[i].transform = RotateVector(verties[i].constTransform, angle);
	}

	Vector2 sub = rp3 - ip3;
	Vector2 add = ip3 + sub;

	pos = rotateCenter + add;



	//pos = rotateCenter - p3 + sub;

	/*bool isSame = false;
	for (int i = 0; i < 4; i++)
	{
		verties[i].transform = RotateVector(verties[i].constTransform, rotate);

		if ((int)verties[i].transform.x == (int)verties[i].constTransform.y ||
			(int)verties[i].transform.y == (int)verties[i].constTransform.x)
			isSame = true;
	}

	if (isSame &&
		size.x != size.y)
	{
		for (int i = 0; i < 4; i++)
			verties[i].transform = { verties[i].constTransform.y,verties[i].constTransform.x };
	}*/

	//頂点基準に回転させた頂点たちを中心基準に戻す
	//基準頂点から中心へのベクトルを計算，回転

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
		sum.x += v.weight * v.constTransform.x;
		sum.y += v.weight * v.constTransform.y;
		sumWeight += v.weight;
	}

	centroid.x = sum.x / static_cast<float>(count);
	centroid.y = sum.y / static_cast<float>(count);
	mass = sumWeight;

	centroid.x = centroid.x > size.x / 2.0f ? size.x / 2.0f : (centroid.x < -size.x / 2.0f ? -size.x / 2.0f : centroid.x);
	centroid.y = centroid.y > size.y / 2.0f ? size.y / 2.0f : (centroid.y < -size.y / 2.0f ? -size.y / 2.0f : centroid.y);

	centroid = RotateVector(centroid, angle);

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

	Verties	copyVerties[4];
	memcpy(copyVerties, verties, sizeof(Verties) * 4);
	bool clear[4] = { true,false ,false ,false };				//配列格納フラグ
	size_t xMin = 0,
		xMax = 0,
		yMin = 0,
		yMax = 0;

	for (int i = 0; i < 4; i++)
	{
		if ((int)copyVerties[i].transform.y < (int)copyVerties[yMin].transform.y)
		{
			yMin = i;
			clear[i] = true;
		}
		else if ((int)copyVerties[i].transform.y > (int)copyVerties[yMax].transform.y)
		{
			yMax = i;
			clear[i] = true;
		}


		if ((int)copyVerties[i].transform.x < (int)copyVerties[xMin].transform.x)
		{
			xMin = i;
			clear[i] = true;
		}
		else if ((int)copyVerties[i].transform.x > (int)copyVerties[xMax].transform.x)
		{
			xMax = i;
			clear[i] = true;
		}
	}

	// 値がかぶっているものがあるとき
	if (!clear[0] || !clear[1] || !clear[2] || !clear[3])
	{
		for (const Verties& vertex : copyVerties)
		{
			//左
			if (vertex.transform.x <= 0)
			{
				//上
				if (vertex.transform.y < 0)
				{
					verties[0] = vertex;
				}
				//下
				else if (vertex.transform.y > 0)
				{
					verties[2] = vertex;
				}
			}
			//右
			else if (vertex.transform.x > 0)
			{
				//上
				if (vertex.transform.y < 0)
				{
					verties[1] = vertex;
				}
				//下
				else if (vertex.transform.y > 0)
				{
					verties[3] = vertex;
				}
			}
		}
	}

	else
	{
		verties[0] = copyVerties[yMin];
		verties[1] = copyVerties[xMax];
		verties[2] = copyVerties[xMin];
		verties[3] = copyVerties[yMax];
	}

}

Vector2 Box::FindRotateCenter()
{
	Vector2 result = { 0,-(float)INFINITE };
	std::vector< Vector2> yMax;

	if (!hitPosWithField.empty())
	{
		for (Vector2 p : hitPosWithField)
		{
			if (result.y <= p.y)
			{
				result = p;
				yMax.push_back(result);
			}
		}
		for (const Vector2& v : yMax)
		{
			if (angleVelocity > 0 || angle - preAngle > 0)
			{
				if (result.x < v.x)
					result = v;
			}
			else if (angleVelocity < 0 || angle - preAngle < 0)
			{
				if (result.x > v.x)
					result = v;
			}
		}

		return result;
	}
	else
		return verties[3].transform + pos;

}

void Box::ShowImGui()
{
	ImGui::Begin("Box");
	ImGui::DragFloat2("position", &pos.x, 0.1f);
	ImGui::DragFloat2("Velocity", &velocity.x, 0.01f);
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
		velocity = { 0,0 };
	ImGui::SliderAngle("angle", &angle);
	ImGui::DragFloat("angle_f", &angle, 0.0001f);
	ImGui::DragFloat("add", &angleVelocity, 0.001f);
	if (ImGui::Button("ResetAngle"))
	{
		angle = 0.0f;
		angleVelocity = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("180"))
		angle = (float)std::numbers::pi;
	ImGui::SameLine();
	if (ImGui::Button("90"))
		angle = 0.5f * (float)std::numbers::pi;
	ImGui::SameLine();
	if (ImGui::Button("60"))
		angle = 60.0f / 180.0f * (float)std::numbers::pi;
	ImGui::Checkbox("Gravity", &isGravity);
	ImGui::DragFloat("Weight", &verties[0].weight, 0.01f);
	ImGui::DragFloat2("Size", &size.x, 0.01f);
	if (ImGui::Button("ReSize"))
	{
		Initialize(pos, size);
	}
	//std::string label = "Transform";
	//for (int i = 0; i < 4; i++)
	//{
	//	ImGui::DragFloat2((label + std::to_string(i)).c_str(), &verties[i].constTransform.x, 0.1f);
	//}
	if (ImGui::TreeNode("Vertex"))
	{
		ImGui::Text("0_x: %+.2f,\t0_y: %+.2f\n 1_x: %+.2f,\t1_y: %+.2f\n 2_x: %+.2f,\t2_y: %+.2f\n 3_x: %+.2f,\t3_y: %+.2f\n",
					verties[0].transform.x, verties[0].transform.y,
					verties[1].transform.x, verties[1].transform.y,
					verties[2].transform.x, verties[2].transform.y,
					verties[3].transform.x, verties[3].transform.y);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("HitPos"))
	{
		for (const Vector2& v : hitPosWithField)
			ImGui::Text("x : %.2f,y : %.2f", v.x, v.y);
		ImGui::TreePop();
	}
	ImGui::End();
}
