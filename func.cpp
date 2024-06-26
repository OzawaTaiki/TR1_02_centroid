#include "func.h"
#include <cmath>
#include <Novice.h>

float lerp(float& t, float max, float min)
{
	return (1.0f - t) * min + t * max;
}

Vector2 lerp(Vector2 min, Vector2 max, float t)
{
	Vector2 result;

	result.x = (1.0f - t) * min.x + t * max.x;
	result.y = (1.0f - t) * min.y + t * max.y;

	return result;
}

void DrawAABB(int _xmin, int _ymin, int _xmax, int _ymax, uint32_t _color)
{
	int lengthX = _xmax - _xmin;
	int lengthY = _ymax - _ymin;

	Novice::DrawBox(_xmin, _ymin, lengthX, lengthY, 0, _color, kFillModeSolid);
}

Vector2 operator+(const Vector2& _vector1, const Vector2& _vector2)
{
	return Vector2(Add(_vector2, _vector1));
}

Vector2 operator-(const Vector2& _vector1, const Vector2& _vector2)
{
	return Vector2(Subtract(_vector2, _vector1));
}

Vector2 operator*(const Vector2& _vector1, float _scalar)
{
	return Vector2(Multiply(_vector1, _scalar));
}

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t)
{
	Vector2 p01 = lerp(p0, p1, t);
	Vector2 p12 = lerp(p1, p2, t);
	Vector2 p = lerp(p01, p12, t);

	return p;
}

float clamp(float a, float min, float max)
{
	if (a < min)
		return min;
	else if (a > max)
		return max;
	else
		return a;
}

float Length(Vector2 a)
{
	return std::sqrtf((a.x * a.x) + (a.y * a.y));
}

Vector2 Add(Vector2 a, Vector2 b)
{
	return { a.x + b.x,a.y + b.y };
}

Vector2 Subtract(Vector2 a, Vector2 b)
{
	return { b.x - a.x, b.y - a.y };
}

Vector2 Multiply(Vector2 a, float num)
{

	return { a.x * num,a.y * num };

}

Vector2 Multiply(Vector2 a, Vector2 b)
{

	return { a.x * b.x,a.y * b.y };

}

float Dot(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}

float Cross(Vector2 a, Vector2 b)
{
	return  a.x * b.y - a.y * b.x;
}

Vector2 Normalize(Vector2 a)
{
	float length = std::sqrtf(a.x * a.x + a.y * a.y);
	if (length != 0)
		return { a.x / length,a.y / length };

	return { 0,0 };
}

Vector2 RotateVector(Vector2 a, float aSin, float aCos)
{
	return { a.x * aCos - a.y * aSin,a.x * aSin + a.y * aCos };
}

Vector2 RotateVector(Vector2 a, float rotate)
{
	return Vector2(RotateVector(a, std::sin(rotate), std::cos(rotate)));
}

bool isCross(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d)
{
	float e = Cross(Subtract(a, b), Subtract(a, c));
	float f = Cross(Subtract(a, b), Subtract(a, d));
	float g = Cross(Subtract(c, d), Subtract(c, a));
	float h = Cross(Subtract(c, d), Subtract(c, b));

	if (e * f > 0 || g * h > 0)
		return false;

	return true;
}

Vector2 getCrossPos(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2 d)
{
	if (!isCross(a, b, c, d))
		return{ 0,0 };

	Vector2 ab = Subtract(a, b);
	Vector2 cd = Subtract(c, d);
	float g = Cross(ab, cd);
	if (g == 0)
		return{ 0,0 };


	float e, f;

	e = Cross(Subtract(a, c), Subtract(c, d)) / g;
	f = Cross(Subtract(a, b), Subtract(c, a)) / g;

	if (e < 0 || e>1 || f < 0 || f>1)
		return{ 0,0 };


	Vector2 result;

	result.x = a.x + e * ab.x;
	result.y = a.y + e * ab.y;

	return result;
}