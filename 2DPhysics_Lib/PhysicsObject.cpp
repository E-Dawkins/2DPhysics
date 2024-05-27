#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
	: mPosition(0, 0)
	, mVelocity(0, 0)
	, mMass(1.f)
	, mRotation(0.f)
{

}

PhysicsObject::PhysicsObject(Vector2D _position, float _mass, float _rotation)
	: mPosition(_position)
	, mVelocity(0, 0)
	, mMass(_mass)
	, mRotation(_rotation)
{
}

void PhysicsObject::Update(float _deltaSeconds)
{
	mPosition += mVelocity * _deltaSeconds;
}

void PhysicsObject::ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
	// Move out of penetration
	if (_collisionInfo.penetration > 0.f)
	{
		mPosition += _collisionInfo.minPenetrationVector * _collisionInfo.penetration;
	}
}
