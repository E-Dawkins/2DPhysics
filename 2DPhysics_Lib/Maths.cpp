#include "Maths.h"

Vector2D P2D_Maths::ProjectPointOnPlane(Vector2D _point, Vector2D _planeCenter, Vector2D _planeNormal)
{
	Vector2D v = _point - _planeCenter;

	float d = Vector2D::Dot(v, _planeNormal);

	Vector2D dn = d * _planeNormal;

	return _point - dn;
}

Vector2D P2D_Maths::ProjectPointOnVector(Vector2D _point, Vector2D _vector)
{
	float dotProduct = Vector2D::Dot(_point, _vector);
	float vectorSquared = Vector2D::Dot(_vector, _vector);

	return (dotProduct / vectorSquared) * _vector;
}

Vector2D P2D_Maths::ClosestPointOnBox(Vector2D _point, Vector2D _boxCenter, Vector2D _boxHalfExtents, Vector2D _boxUp, Vector2D _boxRight)
{
	Vector2D boxToPt = _point - _boxCenter;

	float dotAlongRight = Clamp(-1, 1, Vector2D::Dot(_boxRight, boxToPt) / _boxHalfExtents.X);
	float dotAlongUp = Clamp(-1, 1, Vector2D::Dot(_boxUp, boxToPt) / _boxHalfExtents.Y);

	Vector2D upOffset = _boxUp * (dotAlongUp * _boxHalfExtents.Y);
	Vector2D rightOffset = _boxRight * (dotAlongRight * _boxHalfExtents.X);

	return _boxCenter + upOffset + rightOffset;
}

float P2D_Maths::Deg2Rad(float _angleInDeg)
{
	return _angleInDeg * (pi / 180.f);
}

float P2D_Maths::Rad2Deg(float _angleInRad)
{
	return _angleInRad * (180.f / pi);
}

float P2D_Maths::Clamp(float _min, float _max, float _value)
{
	if (_value < _min) return _min;
	if (_value > _max) return _max;
	return _value;
}

float P2D_Maths::Sign(float _value)
{
	if (_value < 0) return -1;
	if (_value > 0) return 1;
	return 0;
}
