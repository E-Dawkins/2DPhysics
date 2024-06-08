#pragma once
#include "PhysicsObject.h"

class PHYSICS_API CircleCollider : public PhysicsObject
{
public:
	CircleCollider();
	CircleCollider(Vector2D _position, float _mass, float _radius, float _rotation = 0.f);

	// Getters
	const float GetRadius() const { return mRadius; }

	// Setters
	void SetRadius(const float _radius) { mRadius = _radius; }

protected:
	float mRadius;
};

