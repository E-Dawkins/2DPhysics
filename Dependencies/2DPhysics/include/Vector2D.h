#pragma once
#include "DLLDefine.h"

class PHYSICS_API Vector2D
{
public:
	Vector2D();
	Vector2D(float _x, float _y);
	Vector2D(const Vector2D& _other);

	Vector2D operator + (const Vector2D& _lhs);
	Vector2D& operator += (const Vector2D& _rhs);

	Vector2D operator - (const Vector2D& _lhs);
	Vector2D& operator -= (const Vector2D& _rhs);
	Vector2D operator - ();

	Vector2D operator * (const float& _rhs);

	float Magnitude();
	float AbsMagnitude();

	// Normalizes this Vector2D.
	Vector2D& Normalize();

	// Creates a normalized copy of the passed in Vector2D.
	static Vector2D Normalized(Vector2D& _toNormalize);

	static float Dot(Vector2D& _vec1, Vector2D& _vec2);

public:
	float X;
	float Y;
};