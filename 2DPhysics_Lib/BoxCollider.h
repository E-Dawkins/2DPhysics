#pragma once
#include "PhysicsObject.h"

class PHYSICS_API BoxCollider : public PhysicsObject
{
public:
	BoxCollider();
	BoxCollider(Vector2D _position, float _mass, Vector2D _halfExtents, float _rotation = 0.f);

	// Getters
	const Vector2D GetHalfExtents() const				{ return mHalfExtents; }
	const std::array<Vector2D, 4> GetPoints();

	// Setters
	void SetHalfExtents(const Vector2D _extents);

protected:
	void UpdatePoints();
	bool ValidatePoint0();

protected:
	Vector2D mHalfExtents;
	std::array<Vector2D, 4> mPoints;
};

