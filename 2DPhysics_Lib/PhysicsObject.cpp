#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
	: mPosition(0, 0)
	, mVelocity(0, 0)
	, mMass(0.f)
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
