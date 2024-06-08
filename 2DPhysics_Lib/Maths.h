#pragma once
#include "DLLCommon.h"

namespace Physics2D
{
	const float pi = 3.141592653f;

	static Vector2D ProjectPointOnPlane(Vector2D _point, Vector2D _planeCenter, Vector2D _planeNormal)
	{
		Vector2D v = _point - _planeCenter;

		float d = Vector2D::Dot(v, _planeNormal);

		Vector2D dn = d * _planeNormal;

		return _point - dn;
	}

	static float Deg2Rad(float _angleInDeg)
	{
		return _angleInDeg * (pi / 180.f);
	}

	static float Rad2Deg(float _angleInRad)
	{
		return _angleInRad * (180.f / pi);
	}
}