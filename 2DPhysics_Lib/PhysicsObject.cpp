#include "PhysicsObject.h"

#include "CircleCollider.h"
#include "PlaneCollider.h"

#include "Maths.h"

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
	// Impulse magnitude along normal
	//                    -(1+e)((vB - vA) * n)
	// jn = -------------------------------------------------
	//       1/mA + 1/mB + ((rA X n)^2)/iA + ((rB X n)^2)/iB

	Vector2D rA = _collisionInfo.collisionPoints[0] - mPosition;
	Vector2D rB = _collisionInfo.collisionPoints[0] - _otherObject->mPosition;

	Vector2D vA = mVelocity + Vector2D::Cross(mAngularVelocity, rA);
	Vector2D vB = _otherObject->mVelocity + Vector2D::Cross(_otherObject->mAngularVelocity, rB);

	Vector2D rV = (_otherObject->mVelocity + Vector2D::Cross(_otherObject->mAngularVelocity, rB)) -
		(mVelocity - Vector2D::Cross(mAngularVelocity, rA));

	Vector2D normal = Vector2D::Normalize(_collisionInfo.normal);

	float rACrossN = Vector2D::Cross(rA, normal);
	float rBCrossN = Vector2D::Cross(rB, normal);

	float elasticity = (mElasticity + _otherObject->mElasticity) * 0.5f;
	float eTerm = -(1.f + elasticity);

	float massSum = (1.f / mMass) + (1.f / _otherObject->mMass) + ((rACrossN * rACrossN) / mMoment) + ((rBCrossN * rBCrossN) / _otherObject->mMoment);

	float jn = (eTerm * Vector2D::Dot(rV, normal)) / massSum;

	ApplyForce(-jn * normal, rA);
	_otherObject->ApplyForce(jn * normal, rB);

	// Impulse magnitude along tangent
	//                      -((vB - vA) * t)
	// jt = -------------------------------------------------
	//       1/mA + 1/mB + ((rA X t)^2)/iA + ((rB X t)^2)/iB

	rV = (_otherObject->mVelocity + Vector2D::Cross(_otherObject->mAngularVelocity, rB)) -
		(mVelocity - Vector2D::Cross(mAngularVelocity, rA));

	Vector2D tangent = rV - Vector2D::Dot(rV, normal) * normal;
	tangent.Normalized();

	float jt = (-Vector2D::Dot(rV, tangent)) / massSum;

	Vector2D tangentImpulse;

	const float sf = 0.5f; // static friction
	const float df = 0.3f; // dynamic friction

	// Coulomb's Law
	if (std::abs(jt) < jn * sf)
		tangentImpulse = tangent * jt;
	else
		tangentImpulse = tangent * -jn * df;

	ApplyForce(-tangentImpulse, rA);
	_otherObject->ApplyForce(tangentImpulse, rB);

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
bool PhysicsObject::Plane2Plane(PhysicsObject* _plane1, PhysicsObject* _plane2, CollisionInfo& _collisionInfo)
{
	return false;
}

bool PhysicsObject::Circle2Plane(PhysicsObject* _circle, PhysicsObject* _plane, CollisionInfo& _collisionInfo)
{
	CircleCollider* circle = static_cast<CircleCollider*>(_circle);
	PlaneCollider* plane = static_cast<PlaneCollider*>(_plane);

	Vector2D planeToCircle = circle->mPosition - plane->mPosition;
	
	float distFromSurface = Vector2D::Dot(planeToCircle, plane->GetNormal());
	float velDirection = Vector2D::Dot(circle->GetVelocity(), plane->GetNormal());

	// Circle is within collision distance of infinite planes' surface
	// and moving towards plane's surface
	if (distFromSurface <= circle->GetRadius() && velDirection < 0.f)
	{
		Vector2D pointOnPlane = Physics2D::ProjectPointOnPlane(circle->mPosition, plane->mPosition, plane->GetNormal());
		float distFromCenter = Vector2D::Distance(plane->mPosition, pointOnPlane);

		if (distFromCenter <= plane->GetHalfExtent() - circle->GetRadius())
		{
			_collisionInfo.collisionPoints.push_back(pointOnPlane);
			_collisionInfo.penetration = circle->GetRadius() - distFromSurface;

			return true;
		}
	}

	return false;
}

bool PhysicsObject::Plane2Circle(PhysicsObject* _plane, PhysicsObject* _circle, CollisionInfo& _collisionInfo)
{
	return Circle2Plane(_circle, _plane, _collisionInfo);
}

bool PhysicsObject::Circle2Circle(PhysicsObject* _circle1, PhysicsObject* _circle2, CollisionInfo& _collisionInfo)
{
	CircleCollider* circle1 = static_cast<CircleCollider*>(_circle1);
	CircleCollider* circle2 = static_cast<CircleCollider*>(_circle2);

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
#pragma endregion
