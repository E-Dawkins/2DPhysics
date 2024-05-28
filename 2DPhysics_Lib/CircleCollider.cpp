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

bool CircleCollider::CheckCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
	if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(_otherObject))
	{
		return Circle2Circle(otherCircle, _collisionInfo);
	}
	return false;
}

bool CircleCollider::Circle2Circle(CircleCollider* _otherCircle, CollisionInfo& _collisionInfo)
{
	Vector2D toOtherCircle = _otherCircle->mPosition - mPosition;

	float dist = toOtherCircle.AbsMagnitude();
	float combRadius = _otherCircle->mRadius + mRadius;

	if (dist <= combRadius) // the circles are colliding
	{
		float penetration = combRadius - dist;
		Vector2D normal = toOtherCircle.Normalize();

		_collisionInfo.collisionPoints.push_back(mPosition + normal * dist);
		_collisionInfo.penetration = penetration;
		_collisionInfo.normal = -normal;

		return true;
	}

	return false;
}
