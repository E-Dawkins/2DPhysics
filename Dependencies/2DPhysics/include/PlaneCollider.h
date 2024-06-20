#pragma once
#include "PhysicsObject.h"
class PHYSICS_API PlaneCollider : public PhysicsObject
{
public:
	PlaneCollider();
	PlaneCollider(Vector2D _position, float _mass, float _halfExtent, float _rotation = 0.f);

	virtual void ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo) override;

	// Getters
	const float GetHalfExtent() const		{ return mHalfExtent; }
	const Vector2D GetNormal() const		{ return GetLocalUp(); }

	// Setters
	void SetHalfExtent(float& _halfExtent)	{ mHalfExtent = _halfExtent; }

protected:
	float mHalfExtent;
};

