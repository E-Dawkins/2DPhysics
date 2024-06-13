#include "BoxCollider.h"

BoxCollider::BoxCollider()
	: PhysicsObject()
	, mHalfExtents(1, 1)
{
	mMoment = (1.f / 12.f) * GetMass() * (mHalfExtents.X * mHalfExtents.X + mHalfExtents.Y * mHalfExtents.Y);
	mColliderType = BOX;

	UpdatePoints();
}

BoxCollider::BoxCollider(Vector2D _position, float _mass, Vector2D _halfExtents, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mHalfExtents(_halfExtents)
{
	mMoment = (1.f / 12.f) * GetMass() * (mHalfExtents.X * mHalfExtents.X + mHalfExtents.Y * mHalfExtents.Y);
	mColliderType = BOX;

	UpdatePoints();
}

const std::array<Vector2D, 4> BoxCollider::GetPoints()
{
	if (!ValidatePoint0())
	{
		UpdatePoints();
	}

	return mPoints;
}

void BoxCollider::SetHalfExtents(const Vector2D _halfExtents)
{
	mHalfExtents = _halfExtents;
	UpdatePoints();
}

void BoxCollider::UpdatePoints()
{
	mPoints =
	{
		mPosition + Vector2D( (mLocalRight * mHalfExtents.X) + (mLocalUp * mHalfExtents.Y)),
		mPosition + Vector2D( (mLocalRight * mHalfExtents.X) - (mLocalUp * mHalfExtents.Y)),
		mPosition + Vector2D(-(mLocalRight * mHalfExtents.X) + (mLocalUp * mHalfExtents.Y)),
		mPosition + Vector2D(-(mLocalRight * mHalfExtents.X) - (mLocalUp * mHalfExtents.Y)),
	};
}

bool BoxCollider::ValidatePoint0()
{
	return mPoints[0] == (mPosition + Vector2D((mLocalRight * mHalfExtents.X) + (mLocalUp * mHalfExtents.Y)));
}
