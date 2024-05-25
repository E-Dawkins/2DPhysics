#pragma once
#include "DLLDefine.h"

class PHYSICS_API Vector2D
{
public:
	Vector2D();
	Vector2D(float _x, float _y);
	Vector2D(Vector2D& _other);

	Vector2D operator + (const Vector2D& _lhs);
	Vector2D& operator += (const Vector2D& _rhs);

	Vector2D operator - (const Vector2D& _lhs);
	Vector2D& operator -= (const Vector2D& _rhs);

public:
	float X;
	float Y;
};