#include "CircleCollider.h"

CircleCollider::CircleCollider()
	: PhysicsObject()
	, mRadius(1.f)
{
	float radiusInM = mRadius / 100.f;
	mMoment = 0.5f * GetMass() * radiusInM * radiusInM;

	mColliderType = CIRCLE;
}

CircleCollider::CircleCollider(Vector2D _position, float _mass, float _radius, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mRadius(_radius)
{
	float radiusInM = mRadius / 100.f;
	mMoment = 0.5f * GetMass() * radiusInM * radiusInM;

	mColliderType = CIRCLE;
}

bool CircleCollider::Circle2Circle(CircleCollider* _otherCircle, CollisionInfo& _collisionInfo)
{
	Vector2D toOtherCircle = _otherCircle->mPosition - mPosition;

	float dist = toOtherCircle.AbsMagnitude();
	float combRadius = _otherCircle->mRadius + mRadius;

	if (dist <= combRadius) // the circles are colliding
	{
		float penetration = combRadius - dist;
		Vector2D normal = toOtherCircle.Normalized();

		_collisionInfo.collisionPoints.push_back(mPosition + normal * mRadius);
		_collisionInfo.penetration = penetration;
		_collisionInfo.normal = normal;

		return true;
	}

	return false;
}
