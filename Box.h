#pragma once

#include <Vector2.h>
#include <cstdint>


struct Verties
{
	Vector2 constTransform;	//固定 ローカル座標
	Vector2 transform;		//回転したローカル座標
	float weight;			//点における重さの比率
};

class Box {

public:
	Box(int _mapchipSize, uint32_t _color);
	~Box() {};

	void Initialize(const Vector2& _pos);
	void Update();
	void Draw();

	/// <summary>
	/// 接触フラグのセッター
	/// </summary>
	/// <param name="_col">-方向からなら-</param>
	void SetCollisionDir(const Vector2& _col);

	void CollisonWithField();

	///*************************************
	///		ゲッター

	Vector2 GetPosition() { return pos; };
	Vector2 GetSize() { return size; };
	Vector2 GetVelocity() { return velocity; };
	void GetVertiesTransform(Vector2 _verties[]);

	///
	///**************************************

private:
	void Rotate();
	void CalculateCentroid();
	void Gravity();


	Vector2 pos;
	Vector2 size;
	Vector2 velocity;
	Vector2 accelelation;

	//fieldとの接触フラグ
	Vector2 collisionDir;


	float kMapchipSize;
	int rectGH;
	uint32_t color;

	//回転
	float rotate;
	//重心座標
	Vector2 centroid;
	//全体での質量
	float mass;

	Verties verties[4]{};
};
