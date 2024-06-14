#pragma once
#include "DLLCommon.h"

#include <iosfwd> // contains forward declaration for std::ostream

class PHYSICS_API Vector2D
{
public:
	Vector2D();
	Vector2D(float _x, float _y);
	Vector2D(const Vector2D& _other);

	PHYSICS_API friend const Vector2D operator + (const Vector2D& _lhs, const Vector2D& _rhs);
	Vector2D& operator += (const Vector2D& _rhs);

	PHYSICS_API friend const Vector2D operator - (const Vector2D& _lhs, const Vector2D& _rhs);
	Vector2D& operator -= (const Vector2D& _rhs);
	Vector2D operator - () const;

	const Vector2D operator * (const float& _rhs) const;
	PHYSICS_API friend const Vector2D operator * (const float& _lhs, const Vector2D& _rhs);

	const Vector2D operator / (const float& _rhs) const;
	Vector2D& operator /= (const float& _rhs);

	const bool operator == (const Vector2D& _other) const;
	const bool operator != (const Vector2D& _other) const;

	PHYSICS_API friend std::ostream& operator << (std::ostream& os, const Vector2D& _vec);

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

	// Negates this Vector2D
	Vector2D& Negate();

	// Creates a negated copy of the passed in Vector2D
	static Vector2D Negated(Vector2D& _toNegate);

public:
	float X;
	float Y;
};