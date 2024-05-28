#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
	: mPosition(0, 0)
	, mVelocity(0, 0)
	, mMass(1.f)
	, mRotation(0.f)
	, mElasticity(1.f)
{

}

PhysicsObject::PhysicsObject(Vector2D _position, float _mass, float _rotation)
	: mPosition(_position)
	, mVelocity(0, 0)
	, mMass(_mass)
	, mRotation(_rotation)
	, mElasticity(1.f)
{
}

void PhysicsObject::Update(float _deltaSeconds)
{
	mPosition += mVelocity * _deltaSeconds;
}

void PhysicsObject::ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
	// Impulse formula
	//        -(1+e) * vRel * normal
	// j = ----------------------------
	//             1/m1 + 1/m2

	float e = (mElasticity + _otherObject->mElasticity) * 0.5f; // this can be changed to AVG. MIN. MAX. etc.
	Vector2D vRel = mVelocity - _otherObject->mVelocity;
	Vector2D normal = _collisionInfo.normal;
	float m1 = mMass, m2 = _otherObject->mMass;

	float j = (-(1.f + e) * Vector2D::Dot(vRel, normal)) / ((1.f / m1) + (1.f / m2));

	mVelocity += normal * (j / m1);
	_otherObject->mVelocity -= normal * (j / m2);

	// Move out of penetration
	if (_collisionInfo.penetration > 0.f)
	{
		mPosition += _collisionInfo.normal * _collisionInfo.penetration;
	}
}
