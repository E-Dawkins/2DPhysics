#include "BoxCollider.h"

BoxCollider::BoxCollider()
	: PhysicsObject()
	, mExtents(1, 1)
{
	mMoment = (1.f / 12.f) * GetMass() * (mExtents.X * mExtents.X + mExtents.Y * mExtents.Y);

	mColliderType = BOX;
}

BoxCollider::BoxCollider(Vector2D _position, float _mass, Vector2D _extents, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mExtents(_extents)
{
	mMoment = (1.f / 12.f) * GetMass() * (mExtents.X * mExtents.X + mExtents.Y * mExtents.Y);

	mColliderType = BOX;
}
