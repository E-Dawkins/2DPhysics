#include "PhysicsObject.h"

#include "CircleCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"

#include "Maths.h"
#include "PhysicsManager.h"

PhysicsObject::PhysicsObject()
	: mPosition(0, 0)
	, mVelocity(0, 0)
	, mLocalGravity(0, 0)
	, mMass(1.f)
	, mElasticity(1.f)
	, mAngularVelocity(0.f)
	, mMoment(0.f)
	, mLinearDrag(0.3f)
	, mAngularDrag(0.3f)
	, mColliderType(UNKNOWN)
	, mKinematic(false)
	, mUseGravity(false)
{
	SetRotationDegrees(0.f);
	UpdateLocalAxes();

	PhysicsManager::GetInstance().RegisterObject(this);
}

PhysicsObject::PhysicsObject(Vector2D _position, float _mass, float _rotation)
	: mPosition(_position)
	, mVelocity(0, 0)
	, mLocalGravity(0, 0)
	, mMass(_mass)
	, mElasticity(1.f)
	, mAngularVelocity(0.f)
	, mMoment(0.f)
	, mLinearDrag(0.3f)
	, mAngularDrag(0.3f)
	, mColliderType(UNKNOWN)
	, mKinematic(false)
	, mUseGravity(false)
{
	SetRotationDegrees(_rotation);
	UpdateLocalAxes();

	PhysicsManager::GetInstance().RegisterObject(this);
}

void PhysicsObject::Update(float _deltaSeconds)
{
	// Update stored local variables
	if (mLastRotation != mRotation)
	{
		UpdateLocalAxes();
		mLastRotation = mRotation;
	}

	if (mKinematic)
	{
		mVelocity = Vector2D(0, 0);
		mAngularVelocity = 0.f;

		return;
	}

	mVelocity -= mVelocity * mLinearDrag * _deltaSeconds;
	mAngularVelocity -= mAngularVelocity * mAngularDrag * _deltaSeconds;

	if (mVelocity.AbsMagnitude() <= MIN_LINEAR_THRESHOLD)
		mVelocity = Vector2D(0, 0);

	if (std::abs(mAngularVelocity) <= MIN_ANGULAR_THRESHOLD)
		mAngularVelocity = 0.f;

	mPosition += mVelocity * _deltaSeconds;

	if (mUseGravity)
	{
		ApplyForce(mLocalGravity * GetMass() * GetMass() * _deltaSeconds);
	}

	mRotation += mAngularVelocity * _deltaSeconds;
}

void PhysicsObject::ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
	Vector2D normal = Vector2D::Normalized(_collisionInfo.normal);
	Vector2D perp = Vector2D(normal.Y, -normal.X);
	Vector2D contact = _collisionInfo.contactPoint;

	// Determine total velocity of the contact points, both linear and rotational
	// 'r' is the radius from axis to application of force
	float r1 = Vector2D::Dot(contact - mPosition, -perp);
	float r2 = Vector2D::Dot(contact - _otherObject->mPosition, perp);

	// Velocity of contact point on each object
	float v1 = Vector2D::Dot(mVelocity, normal) - r1 * mAngularVelocity;
	float v2 = Vector2D::Dot(_otherObject->mVelocity, normal) + r2 * _otherObject->mAngularVelocity;

	if (v1 > v2) // moving closer to each other
	{
		// Effective mass at the contact point for each object
		// i.e. how far the point will move due to the applied force
		float mass1 = 1.f / (1.f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.f / (1.f / _otherObject->GetMass() + (r2 * r2) / _otherObject->GetMoment());

		float elasticity = (GetElasticity() + _otherObject->GetElasticity()) * 0.5f; // average elasticity

		Vector2D force = (1.f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		ApplyForce(-force, contact - mPosition);
		_otherObject->ApplyForce(force, contact - _otherObject->mPosition);

		// Move objects out of penetration
		if (_collisionInfo.penetration > 0)
			ApplyContactForces(_otherObject, _collisionInfo);
	}
}

void PhysicsObject::ApplyForce(Vector2D _force, const Vector2D _contact)
{
	mVelocity += _force / GetMass();
	mAngularVelocity += Vector2D::Cross(_contact, _force) / GetMoment();
}

void PhysicsObject::ApplyContactForces(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
	float mass2 = _otherObject->GetMass();
	float factor1 = mass2 / (GetMass() + mass2);

	if (!mKinematic)
		mPosition -= factor1 * _collisionInfo.normal * _collisionInfo.penetration;

	if (!_otherObject->mKinematic)
		_otherObject->mPosition += (1.f - factor1) * _collisionInfo.normal * _collisionInfo.penetration;
}

void PhysicsObject::SetRotationDegrees(const float _rotation)
{ 
	mRotation = P2D_Maths::Deg2Rad(_rotation);
	UpdateLocalAxes();
}

void PhysicsObject::UpdateLocalAxes()
{
	float sn = sinf(mRotation);
	float cs = cosf(mRotation);

	mLocalRight = Vector2D(cs, sn);
	mLocalUp = Vector2D(-sn, cs);
}
