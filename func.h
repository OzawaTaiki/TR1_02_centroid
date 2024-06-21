#pragma once
#include <Vector2.h>

float lerp(float& t, float max, float min);

Vector2 lerp(Vector2 min, Vector2 max, float t);

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t);

float clamp(float a, float min, float max);

float Length(Vector2 a);

Vector2 Add(Vector2 a, Vector2 b);

Vector2 Subtract(Vector2 a, Vector2 b);

Vector2 Multiply(Vector2 a, float num);

Vector2 Multiply(Vector2 a, Vector2 b);

float Dot(Vector2 a, Vector2 b);

float Cross(Vector2 a, Vector2 b);

Vector2 Normalize(Vector2 a);

Vector2 RotateVector(Vector2 a, float aSin, float aCos);

Vector2 RotateVector(Vector2 a, float rotate);

bool isCross(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d);

Vector2 getCrossPos(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2 d);

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t);

Vector2 lerp(Vector2 min, Vector2 max, float t);