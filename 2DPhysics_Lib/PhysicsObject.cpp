#include "PhysicsObject.h"

#include "CircleCollider.h"

PhysicsObject::PhysicsObject()
	: mPosition(0, 0)
	, mVelocity(0, 0)
	, mMass(1.f)
	, mRotation(0.f)
	, mElasticity(1.f)
	, mAngularVelocity(0.f)
	, mMoment(0.f)
	, mColliderType(UNKNOWN)
{
	RegisterCollisionChecks();
}

PhysicsObject::PhysicsObject(Vector2D _position, float _mass, float _rotation)
	: mPosition(_position)
	, mVelocity(0, 0)
	, mMass(_mass)
	, mRotation(_rotation)
	, mElasticity(1.f)
	, mAngularVelocity(0.f)
	, mMoment(0.f)
	, mColliderType(UNKNOWN)
{
	RegisterCollisionChecks();
}

void PhysicsObject::Update(float _deltaSeconds)
{
	mPosition += mVelocity * _deltaSeconds;
	mRotation += mAngularVelocity * _deltaSeconds;
}

bool PhysicsObject::CheckCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
	int funcIndex = mColliderType + COLLIDER_TYPE_MAX * _otherObject->mColliderType;

	if (funcIndex < COLLIDER_TYPE_MAX * COLLIDER_TYPE_MAX) // valid function index
	{
		return (this->*mCollisionChecks[funcIndex])(this, _otherObject, _collisionInfo);
	}

	return false;
}

void PhysicsObject::ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo)
{
    Vector2D normal = Vector2D::Normalize(_collisionInfo.normal);
    Vector2D perp = Vector2D(normal.Y, -normal.X); // vector perpendicular to the normal

    // Determine total velocity of the contact points, both linear and rotational
    // 'r' is the radius from axis to application of force
    float r1 = Vector2D::Dot(_collisionInfo.collisionPoints[0] - mPosition, -perp);
    float r2 = Vector2D::Dot(_collisionInfo.collisionPoints[0] - _otherObject->mPosition, perp);

    // Velocity of contact point on this object
    float v1 = Vector2D::Dot(mVelocity, normal) - r1 * mAngularVelocity;
    // Velocity of contact point on other object
    float v2 = Vector2D::Dot(_otherObject->mVelocity, normal) + r2 * _otherObject->mAngularVelocity;

    if (v1 > v2) // moving closer to each other
    {
        // Effective mass at the contact point for each object
        // i.e. how far the point will move due to the applied force
        float mass1 = 1.f / (1.f / GetMass() + (r1 * r1) / GetMoment());
        float mass2 = 1.f / (1.f / _otherObject->GetMass() + (r2 * r2) / _otherObject->GetMoment());

        float elasticity = (GetElasticity() + _otherObject->GetElasticity()) * 0.5f;

        Vector2D force = -(1.f + elasticity) * mass1 * mass2 /
            (mass1 + mass2) * (v1 - v2) * normal;

        // Apply equal and opposite force
        ApplyForce(force, _collisionInfo.collisionPoints[0] - mPosition);
        _otherObject->ApplyForce(-force, _collisionInfo.collisionPoints[0] - _otherObject->mPosition);
    }

	// Move out of penetration
	if (_collisionInfo.penetration > 0.f)
	{
		ApplyContactForces(_otherObject, normal, _collisionInfo.penetration);
	}
}

void PhysicsObject::ApplyForce(Vector2D _force, const Vector2D _contact)
{
	mVelocity += _force / GetMass();
	mAngularVelocity += (_force.Y * _contact.X - _force.X * _contact.Y) / GetMoment();
}

void PhysicsObject::ApplyContactForces(PhysicsObject* _otherObject, Vector2D _normal, float _penetration)
{
	float mass2 = _otherObject->mMass;
	float factor1 = mass2 / (mMass + mass2);

	mPosition -= factor1 * _normal * _penetration;
	_otherObject->mPosition += (1.f - factor1) * _normal * _penetration;
}

void PhysicsObject::RegisterCollisionChecks()
{
	//			PLANE	CIRCLE
	// PLANE
	// CIRCLE

	mCollisionChecks = {
		&PhysicsObject::Plane2Plane, &PhysicsObject::Circle2Plane,
		&PhysicsObject::Plane2Circle, &PhysicsObject::Circle2Circle
	};
}

#pragma region CollisionChecks
bool PhysicsObject::Circle2Circle(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo)
{
	CircleCollider* circle1 = static_cast<CircleCollider*>(_object1);
	CircleCollider* circle2 = static_cast<CircleCollider*>(_object2);

	Vector2D toOtherCircle = circle2->mPosition - circle1->mPosition;

	float dist = toOtherCircle.AbsMagnitude();
	float combRadius = circle2->GetRadius() + circle1->GetRadius();

	if (dist <= combRadius) // the circles are colliding
	{
		float penetration = combRadius - dist;
		Vector2D normal = toOtherCircle.Normalized();

		_collisionInfo.collisionPoints.push_back(circle1->mPosition + normal * circle1->GetRadius());
		_collisionInfo.penetration = penetration;
		_collisionInfo.normal = normal;

		return true;
	}

	return false;
}

bool PhysicsObject::Circle2Plane(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo)
{
	return false;
}

bool PhysicsObject::Plane2Circle(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo)
{
	return false;
}

bool PhysicsObject::Plane2Plane(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo)
{
	return false;
}
#pragma endregion
