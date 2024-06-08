#include "CircleCollider.h"

CircleCollider::CircleCollider()
	: PhysicsObject()
	, mRadius(1.f)
{
	mMoment = 0.5f * GetMass() * mRadius * mRadius;

	mColliderType = CIRCLE;
}

CircleCollider::CircleCollider(Vector2D _position, float _mass, float _radius, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mRadius(_radius)
{
	mMoment = 0.5f * GetMass() * mRadius * mRadius;

	mColliderType = CIRCLE;
}
