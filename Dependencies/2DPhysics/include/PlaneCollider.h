#pragma once
#include "PhysicsObject.h"
class PHYSICS_API PlaneCollider : public PhysicsObject
{
public:
	PlaneCollider();
	PlaneCollider(Vector2D _position, float _mass, Vector2D _normal, float _halfExtent, float _rotation = 0.f);

	virtual void ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);

	// Getters
	const Vector2D GetNormal() const		{ return mNormal; }
	const float GetHalfExtent() const		{ return mHalfExtent; }

	// Setters
	void SetNormal(Vector2D& _normal)		{ mNormal = _normal; }
	void SetHalfExtent(float& _halfExtent)	{ mHalfExtent = _halfExtent; }

protected:
	Vector2D mNormal;
	float mHalfExtent;
};

