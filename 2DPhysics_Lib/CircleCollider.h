#pragma once
#include "PhysicsObject.h"

class PHYSICS_API CircleCollider : public PhysicsObject
{
public:
	CircleCollider();
	CircleCollider(Vector2D _position, float _mass, float _radius, float _rotation = 0.f);

	virtual bool CheckCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);

	// Getters
	const float GetRadius() const { return mRadius; }

	// Setters
	void SetRadius(const float _radius) { mRadius = _radius; }

protected:
	bool Circle2Circle(CircleCollider* _otherCircle, CollisionInfo& _collisionInfo);

protected:
	float mRadius;
};

