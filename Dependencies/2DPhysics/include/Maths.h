#pragma once
#include "DLLCommon.h"
#include "Vector2D.h"

const float pi = 3.141592653f;

class PHYSICS_API P2D_Maths
{
public:
	static Vector2D ProjectPointOnPlane(Vector2D _point, Vector2D _planeCenter, Vector2D _planeNormal);
	static Vector2D ProjectPointOnVector(Vector2D _point, Vector2D _vector);
	static Vector2D ClosestPointOnBox(Vector2D _point, Vector2D _boxCenter, Vector2D _boxHalfExtents, Vector2D _boxUp, Vector2D _boxRight);

	static float Deg2Rad(float _angleInDeg);
	static float Rad2Deg(float _angleInRad);
	static float Clamp(float _min, float _max, float _value);
	static float Sign(float _value);
};