#include "PhysicsObject.h"

#include "CircleCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"

#include "Maths.h"

PhysicsObject::PhysicsObject()
	: mPosition(0, 0)
	, mVelocity(0, 0)
	, mMass(1.f)
	, mElasticity(1.f)
	, mAngularVelocity(0.f)
	, mMoment(0.f)
	, mColliderType(UNKNOWN)
	, mKinematic(false)
{
	SetRotationDegrees(0.f);
	UpdateLocalAxes();
	RegisterCollisionChecks();
}

PhysicsObject::PhysicsObject(Vector2D _position, float _mass, float _rotation)
	: mPosition(_position)
	, mVelocity(0, 0)
	, mMass(_mass)
	, mElasticity(1.f)
	, mAngularVelocity(0.f)
	, mMoment(0.f)
	, mColliderType(UNKNOWN)
	, mKinematic(false)
{
	SetRotationDegrees(_rotation);
	UpdateLocalAxes();
	RegisterCollisionChecks();
}

void PhysicsObject::Update(float _deltaSeconds)
{
	// Update stored local variables
	if (mLastRotation != mRotation)
	{
		UpdateLocalAxes();
	}

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
	// TODO - replace these with actual static/dynamic friction
	const float sf = 0.5f;
	const float df = 0.3f;

	Vector2D normal = Vector2D::Normalized(_collisionInfo.normal);

	float wA = mAngularVelocity;
	float wB = _otherObject->mAngularVelocity;

	Vector2D lA = mVelocity;
	Vector2D lB = _otherObject->mVelocity;

	float elasticity = mElasticity * _otherObject->mElasticity;
	float eTerm = -(1.f + elasticity);

	for (Vector2D& pt : _collisionInfo.collisionPoints)
	{
		// Impulse magnitude along normal
		//                    -(1+e)((vB - vA) * n)
		// jn = -------------------------------------------------
		//       1/mA + 1/mB + ((rA X n)^2)/iA + ((rB X n)^2)/iB

		Vector2D rA = pt - mPosition;
		Vector2D rB = pt - _otherObject->mPosition;

		Vector2D vA = lA + Vector2D::Cross(wA, rA);
		Vector2D vB = lB + Vector2D::Cross(wB, rB);

		Vector2D rV = vB - vA;

		float rACrossN = Vector2D::Cross(rA, normal);
		float rBCrossN = Vector2D::Cross(rB, normal);

		float invMass1 = (1.f / GetMass()), invMass2 = (1.f / _otherObject->GetMass());
		float massSum = invMass1 + invMass2;

		float jn = (eTerm * Vector2D::Dot(rV, normal)) / massSum;
		jn /= _collisionInfo.collisionPoints.size();

		ApplyForce(-jn * normal, rA);
		_otherObject->ApplyForce(jn * normal, rB);

		// Impulse magnitude along tangent
		//                      -((vB - vA) * t)
		// jt = -------------------------------------------------
		//       1/mA + 1/mB + ((rA X t)^2)/iA + ((rB X t)^2)/iB

		rV = vB - vA;

		Vector2D tangent = rV - Vector2D::Dot(rV, normal) * normal;
		tangent.Normalize();

		float torque1 = ((rACrossN * rACrossN) / GetMoment()), torque2 = ((rBCrossN * rBCrossN) / _otherObject->GetMoment());
		massSum += torque1 + torque2;

		float jt = (-Vector2D::Dot(rV, tangent)) / massSum;
		jt /= _collisionInfo.collisionPoints.size();

		Vector2D tangentImpulse;

		// Coulomb's Law
		if (std::abs(jt) < jn * sf)
			tangentImpulse = tangent * jt;
		else
			tangentImpulse = tangent * -jn * df;

		ApplyForce(-tangentImpulse, rA);
		_otherObject->ApplyForce(tangentImpulse, rB);
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
	mAngularVelocity += Vector2D::Cross(_force, _contact) / GetMoment();
}

void PhysicsObject::ApplyContactForces(PhysicsObject* _otherObject, Vector2D _normal, float _penetration)
{
	float mass2 = _otherObject->GetMass();
	float factor1 = mass2 / (GetMass() + mass2);

	if (!mKinematic)
		mPosition -= factor1 * _normal * _penetration;

	if (!_otherObject->mKinematic)
		_otherObject->mPosition += (1.f - factor1) * _normal * _penetration;
}

void PhysicsObject::SetRotationDegrees(const float _rotation)
{ 
	mRotation = Physics2D::Deg2Rad(_rotation);
	UpdateLocalAxes();
}

void PhysicsObject::UpdateLocalAxes()
{
	mLastRotation = mRotation;

	mLocalUp = Vector2D::AngleToUnitVector(Physics2D::Rad2Deg(mRotation));
	mLocalRight = Vector2D::PerpendicularVector(mLocalUp).Normalize();
}

void PhysicsObject::RegisterCollisionChecks()
{
	//			PLANE	CIRCLE	BOX
	// PLANE
	// CIRCLE
	// BOX

	mCollisionChecks = {
		&PhysicsObject::Plane2Plane,	&PhysicsObject::Circle2Plane,	&PhysicsObject::Box2Plane,
		&PhysicsObject::Plane2Circle,	&PhysicsObject::Circle2Circle,	&PhysicsObject::Box2Circle,
		&PhysicsObject::Plane2Box,		&PhysicsObject::Circle2Box,		&PhysicsObject::Box2Box,
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
	
	float distFromSurface = std::abs(Vector2D::Dot(planeToCircle, plane->GetNormal()));
	float velDirection = Vector2D::Dot(circle->GetVelocity(), plane->GetNormal());

	// Circle is within collision distance of infinite planes' surface
	// and moving towards plane's surface
	if (distFromSurface <= circle->GetRadius() && velDirection < 0.f)
	{
		Vector2D pointOnPlane = Physics2D::ProjectPointOnPlane(circle->mPosition, plane->mPosition, plane->GetNormal());
		float distFromCenter = Vector2D::Distance(plane->mPosition, pointOnPlane) - circle->GetRadius();

		if (distFromCenter <= plane->GetHalfExtent())
		{
			_collisionInfo.collisionPoints.push_back(pointOnPlane);
			_collisionInfo.penetration = circle->GetRadius() - distFromSurface;

			return true;
		}
	}

	return false;
}

bool PhysicsObject::Box2Plane(PhysicsObject* _box, PhysicsObject* _plane, CollisionInfo& _collisionInfo)
{
	BoxCollider* box = static_cast<BoxCollider*>(_box);
	PlaneCollider* plane = static_cast<PlaneCollider*>(_plane);

	// Check if any point of the box is on the other side of plane's normal
	const auto points = box->GetPoints();

	for (int i = 0; i < 4; i++)
	{
		Vector2D pt = points[i];

		Vector2D planeToPoint = pt - plane->GetPosition();
		float distFromPlane = Vector2D::Dot(planeToPoint, plane->GetNormal());

		Vector2D localContact = pt - mPosition;
		Vector2D relativeVel = mVelocity + mAngularVelocity
			* Vector2D(-localContact.Y, localContact.X);
		float velocityIntoPlane = Vector2D::Dot(relativeVel, plane->GetNormal());

		// The point is behind the plane, and moving towards the plane
		if (distFromPlane < 0.f && velocityIntoPlane < 0.f)
		{
			_collisionInfo.collisionPoints.push_back(pt);
			
			if (distFromPlane < _collisionInfo.penetration)
			{
				_collisionInfo.penetration = std::abs(distFromPlane);
			}

			_collisionInfo.normal = -plane->GetNormal();
		}
	}

	return (_collisionInfo.collisionPoints.size() > 0);
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
		Vector2D normal = toOtherCircle.Normalize();

		_collisionInfo.collisionPoints.push_back(circle1->mPosition + normal * circle1->GetRadius());
		_collisionInfo.penetration = penetration;
		_collisionInfo.normal = normal;

		return true;
	}

	return false;
}
bool PhysicsObject::Box2Circle(PhysicsObject* _box, PhysicsObject* _circle, CollisionInfo& _collisionInfo)
{
	return false; // TODO
}
bool PhysicsObject::Plane2Box(PhysicsObject* _plane, PhysicsObject* _box, CollisionInfo& _collisionInfo)
{
	return Box2Plane(_box, _plane, _collisionInfo);
}
bool PhysicsObject::Circle2Box(PhysicsObject* _circle, PhysicsObject* _box, CollisionInfo& _collisionInfo)
{
	return Box2Circle(_box, _circle, _collisionInfo);
}
bool PhysicsObject::Box2Box(PhysicsObject* _box1, PhysicsObject* _box2, CollisionInfo& _collisionInfo)
{
	return false; // TODO
}
#pragma endregion
