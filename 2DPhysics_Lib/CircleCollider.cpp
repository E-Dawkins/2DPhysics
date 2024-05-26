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
	if (CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(_otherObject))
	{
		return Circle2Circle(otherCircle);
	}
	return false;
}

bool CircleCollider::Circle2Circle(CircleCollider* _otherCircle)
{
	float dist = (_otherCircle->mPosition - mPosition).AbsMagnitude();
	float combRadius = _otherCircle->mRadius + mRadius;

	return dist <= combRadius;
}
