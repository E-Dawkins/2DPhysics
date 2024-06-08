#include "PlaneCollider.h"

PlaneCollider::PlaneCollider()
	: PhysicsObject()
	, mNormal(0, 1)
	, mHalfExtent(0.5f)
{
	float halfExtentInM = mHalfExtent / 100.f;
	mMoment = (1.f / 3.f) * GetMass() * halfExtentInM * halfExtentInM;

	mColliderType = PLANE;
}

PlaneCollider::PlaneCollider(Vector2D _position, float _mass, Vector2D _normal, float _halfExtent, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mNormal(_normal)
	, mHalfExtent(_halfExtent)
{
	float halfExtentInM = mHalfExtent / 100.f;
	mMoment = (1.f / 3.f) * GetMass() * halfExtentInM * halfExtentInM;

	mColliderType = PLANE;
}

void PlaneCollider::ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
    // The position at which we will apply the force
    // relative to the object's center of mass
    Vector2D localContact = _collisionInfo.collisionPoints[0] - _otherObject->GetPosition();

    // The plane has no velocity, so the relative velocity
    // is just the other object's velocity at contact point
    Vector2D relativeVel = _otherObject->GetVelocity() + _otherObject->GetAngularVelocity()
        * Vector2D(-localContact.Y, localContact.X);

    float velocityIntoPlane = Vector2D::Dot(relativeVel, mNormal);
    float elasticity = (GetElasticity() + _otherObject->GetElasticity()) * 0.5f;

    // Perpendicular distance we apply the force to relative
    // to the object's center of mass, i.e. Torque = F * r
    float r = Vector2D::Dot(localContact, Vector2D(mNormal.Y, -mNormal.X));

    // "Effective mass" - combination of moment of inertia and mass,
    // tells us contact points' velocity change from the applied force
    float mass0 = 1.f / (1.f / _otherObject->GetMass() + (r * r) / _otherObject->GetMoment());

    float j = -(1 + elasticity) * velocityIntoPlane * mass0;

    Vector2D force = mNormal * j;

    _otherObject->ApplyForce(force, _collisionInfo.collisionPoints[0] - _otherObject->GetPosition());

    if (_collisionInfo.penetration > 0.f)
    {
        ApplyContactForces(_otherObject, mNormal, _collisionInfo.penetration);
    }

    _collisionInfo.normal = mNormal;
}
