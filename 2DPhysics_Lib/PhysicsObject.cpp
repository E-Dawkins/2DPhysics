#include "PhysicsObject.h"

#include "CircleCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"

#include "Maths.h"

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
	RegisterCollisionChecks();
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
	RegisterCollisionChecks();
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
		// Use a referece point on the plane to determine if circle is within plane bounds
		Vector2D pointOnPlane = P2D_Maths::ProjectPointOnPlane(circle->mPosition, plane->mPosition, plane->GetNormal());
		float distFromCenter = Vector2D::Distance(plane->mPosition, pointOnPlane) - circle->GetRadius();

		if (distFromCenter <= plane->GetHalfExtent())
		{
			// Use the actual point on the circles' surface for collision resolution
			_collisionInfo.contactPoint = circle->GetPosition() + (plane->GetNormal() * -circle->GetRadius());
			plane->ResolveCollision(circle, _collisionInfo);

			return true;
		}
	}

	return false;
}

bool PhysicsObject::Box2Plane(PhysicsObject* _box, PhysicsObject* _plane, CollisionInfo& _collisionInfo)
{
	BoxCollider* box = static_cast<BoxCollider*>(_box);
	PlaneCollider* plane = static_cast<PlaneCollider*>(_plane);

	Vector2D contact = Vector2D(0, 0);
	int numContacts = 0;

	// Check if any point of the box is on the other side of plane's normal
	const auto& points = box->GetPoints();
	for (int i = 0; i < 4; i++)
	{
		Vector2D pt = points[i];

		Vector2D planeToPoint = pt - plane->GetPosition();
		float distFromPlane = Vector2D::Dot(planeToPoint, plane->GetNormal());

		Vector2D localContact = pt - box->GetPosition();
		Vector2D relativeVel = box->GetVelocity() + box->GetAngularVelocity()
			* Vector2D(-localContact.Y, localContact.X);
		float velocityIntoPlane = Vector2D::Dot(relativeVel, plane->GetNormal());

		// The point is behind the plane, and moving towards the plane
		if (distFromPlane < 0.f && velocityIntoPlane < 0.f)
		{
			contact += pt;
			numContacts++;
		}
	}

	if (numContacts > 0)
	{
		_collisionInfo.contactPoint = contact / (float)numContacts;
		plane->ResolveCollision(box, _collisionInfo);

		return true;
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
		Vector2D normal = toOtherCircle.Normalize();

		_collisionInfo.contactPoint = circle1->mPosition + normal * circle1->GetRadius();
		_collisionInfo.penetration = penetration;
		_collisionInfo.normal = normal;

		circle1->ResolveCollision(circle2, _collisionInfo);

		return true;
	}

	return false;
}
bool PhysicsObject::Box2Circle(PhysicsObject* _box, PhysicsObject* _circle, CollisionInfo& _collisionInfo)
{
	BoxCollider* box = static_cast<BoxCollider*>(_box);
	CircleCollider* circle = static_cast<CircleCollider*>(_circle);

	Vector2D circlePos = circle->GetPosition();
	Vector2D ptOnBox = P2D_Maths::ClosestPointOnBox(circlePos, box->GetPosition(),
		box->GetHalfExtents(), box->GetLocalUp(), box->GetLocalRight());

	float distToPt = Vector2D::Distance(circlePos, ptOnBox);
	float circleRadius = circle->GetRadius();

	if (distToPt <= circleRadius)
	{
		Vector2D normal = circlePos - ptOnBox;
		_collisionInfo.contactPoint = ptOnBox;
		_collisionInfo.penetration = circleRadius - distToPt;
		_collisionInfo.normal = normal.Normalize();

		box->ResolveCollision(circle, _collisionInfo);

		return true;
	}

	return false;
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
	BoxCollider* box1 = static_cast<BoxCollider*>(_box1);
	BoxCollider* box2 = static_cast<BoxCollider*>(_box2);

	Vector2D contact = { 0, 0 }, normal = { 0, 0 };
	int numContacts = 0;
	float pen = FLT_MAX;

	box1->CheckBoxCorners(box2, contact, numContacts, pen, normal);

	if (box2->CheckBoxCorners(box1, contact, numContacts, pen, normal))
	{
		normal = -normal;
	}

	if (contact != Vector2D(0, 0))
	{
		_collisionInfo.contactPoint = contact / (float)numContacts;
		_collisionInfo.normal = normal;
		_collisionInfo.penetration = pen;

		box1->ResolveCollision(box2, _collisionInfo);

		return true;
	}

	return false;
}
#pragma endregion
