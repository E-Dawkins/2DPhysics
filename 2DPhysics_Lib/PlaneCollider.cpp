#include "PlaneCollider.h"

PlaneCollider::PlaneCollider()
	: PhysicsObject()
	, mHalfExtent(0.5f)
{
	mMoment = (1.f / 3.f) * GetMass() * mHalfExtent * mHalfExtent;

	mColliderType = PLANE;

    mKinematic = true;
}

PlaneCollider::PlaneCollider(Vector2D _position, float _mass, float _halfExtent, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mHalfExtent(_halfExtent)
{
	mMoment = (1.f / 3.f) * GetMass() * mHalfExtent * mHalfExtent;

	mColliderType = PLANE;

    mKinematic = true;
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

    float velocityIntoPlane = Vector2D::Dot(relativeVel, mLocalUp);
    float elasticity = (GetElasticity() + _otherObject->GetElasticity()) * 0.5f;

    // Perpendicular distance we apply the force to relative
    // to the object's center of mass, i.e. Torque = F * r
    float r = Vector2D::Dot(localContact, Vector2D(mLocalUp.Y, -mLocalUp.X));

    // "Effective mass" - combination of moment of inertia and mass,
    // tells us contact points' velocity change from the applied force
    float mass0 = 1.f / (1.f / _otherObject->GetMass() + (r * r) / _otherObject->GetMoment());

    float j = -(1 + elasticity) * velocityIntoPlane * mass0;

    Vector2D force = mLocalUp * j;

    _otherObject->ApplyForce(force, localContact);

    // The penetration is simply the dot product of the contact point and the planes' normal then
    // subtracting the distance from the world origin, i.e. the dot product of the position and the normal.
    _collisionInfo.normal = mLocalUp;
    _collisionInfo.penetration = -(Vector2D::Dot(_collisionInfo.collisionPoints[0], mLocalUp) - Vector2D::Dot(mPosition, mLocalUp));
    if (_collisionInfo.penetration > 0.f)
    {
        ApplyContactForces(_otherObject, _collisionInfo);
    }
}
