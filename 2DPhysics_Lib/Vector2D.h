#pragma once
#include "DLLCommon.h"

class PHYSICS_API Vector2D
{
public:
	Vector2D();
	Vector2D(float _x, float _y);
	Vector2D(const Vector2D& _other);

	PHYSICS_API friend Vector2D operator + (Vector2D _lhs, Vector2D _rhs);
	Vector2D& operator += (Vector2D _rhs);

	Vector2D operator - (Vector2D _lhs);
	Vector2D& operator -= (Vector2D _rhs);
	Vector2D operator - ();

	Vector2D operator * (float _rhs);
	PHYSICS_API friend Vector2D operator * (float _lhs, Vector2D _rhs);

	Vector2D operator / (float _rhs);
	Vector2D& operator /= (float _rhs);

	bool operator == (Vector2D _other);
	bool operator != (Vector2D _other);

	float Magnitude();
	float AbsMagnitude();

	// Normalizes this Vector2D
	Vector2D& Normalize();

	// Creates a normalized copy of the passed in Vector2D
	static Vector2D Normalized(Vector2D& _toNormalize);

	static float Dot(Vector2D _vec1, Vector2D _vec2);

	static float Cross(const Vector2D& _vec1, const Vector2D& _vec2);
	static Vector2D Cross(const float& _s, const Vector2D& _vec);
	static Vector2D Cross(const Vector2D& _vec, const float& _s);

	// Converts an angle (degrees) to a unit vector
	static Vector2D AngleToUnitVector(const float& _angle);

	static float Distance(const Vector2D& _vec1, const Vector2D& _vec2);

	static Vector2D PerpendicularVector(const Vector2D& _vec);

public:
	float X;
	float Y;
};