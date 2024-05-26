#include "CircleCollider.h"

CircleCollider::CircleCollider()
	: PhysicsObject()
	, mRadius(1.f)
{

}

CircleCollider::CircleCollider(Vector2D _position, float _mass, float _radius, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mRadius(_radius)
{

}

bool CircleCollider::CheckCollision(PhysicsObject* _otherObject)
{
	mCollisionInfo = CollisionInfo();

	if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(_otherObject))
	{
		return Circle2Circle(otherCircle);
	}
	return false;
}

bool CircleCollider::Circle2Circle(CircleCollider* _otherCircle)
{
	Vector2D toOtherCircle = _otherCircle->mPosition - mPosition;

	float dist = toOtherCircle.AbsMagnitude();
	float combRadius = _otherCircle->mRadius + mRadius;

	if (dist <= combRadius)
	{
		float penetration = combRadius - dist;

		mCollisionInfo.collisionPoints.push_back(toOtherCircle * penetration);
		mCollisionInfo.minPenetration = penetration;

		return true;
	}

	return false;
}
