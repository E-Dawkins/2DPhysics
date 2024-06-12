#pragma once
#include "PhysicsObject.h"
class BoxCollider : public PhysicsObject
{
	BoxCollider();
	BoxCollider(Vector2D _position, float _mass, Vector2D _extents, float _rotation = 0.f);

	// Getters
	const Vector2D GetExtents() const			{ return mExtents; }

	// Setters
	void SetExtents(const Vector2D _extents)	{ mExtents = _extents; }

protected:
	Vector2D mExtents;
};

