#include "PhysicsManager.h"
#include "CircleCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"

#include <chrono>

typedef std::chrono::high_resolution_clock hrc;
using std::chrono::duration_cast;
using std::chrono::duration;

PhysicsManager* PhysicsManager::mInstance = nullptr;

PhysicsManager::PhysicsManager()
{
	mPhysicsThread = {};
	mObjects = {};
	mTickRate = 60;
	mDeltaSeconds = 1.f / (float)mTickRate;
	mQuit = false;

	RegisterCollisionChecks();
}

PhysicsManager::~PhysicsManager()
{
	mObjects.clear();

	delete mInstance;
}

void PhysicsManager::Init()
{
	mPhysicsThread = std::thread(&PhysicsManager::FixedUpdate, this);
}

void PhysicsManager::Quit()
{
	mQuit = true;
	mPhysicsThread.join();
}

void PhysicsManager::FixedUpdate()
{
	static hrc::time_point lastTime = hrc::now();

	while (!mQuit)
	{
		const hrc::time_point currentTime = hrc::now();
		const auto elapsedTime = currentTime - lastTime;

		const float deltaSeconds = duration_cast<duration<float>>(elapsedTime).count();

		if (deltaSeconds >= mDeltaSeconds)
		{
			UpdateObjs();
			CheckCollisions();

			lastTime = currentTime;
		}
	}
}

void PhysicsManager::UpdateObjs()
{
	for (auto& obj : mObjects)
	{
		obj->Update(mDeltaSeconds);
	}
}

void PhysicsManager::CheckCollisions()
{
	for (int outer = 0; outer < (int)mObjects.size() - 1; outer++)
	{
		for (int inner = outer + 1; inner < (int)mObjects.size(); inner++)
		{
			PhysicsObject* object1 = mObjects[outer];
			PhysicsObject* object2 = mObjects[inner];

			CollisionInfo collInfo = {};
			if (CheckCollision(object1, object2, collInfo))
			{
				// TODO
			}
		}
	}
}

bool PhysicsManager::CheckCollision(PhysicsObject* _obj1, PhysicsObject* _obj2, CollisionInfo& _collisionInfo)
{
	int funcIndex = _obj1->GetColliderType() + COLLIDER_TYPE_MAX * _obj2->GetColliderType();

	if (funcIndex < COLLIDER_TYPE_MAX * COLLIDER_TYPE_MAX) // valid function index
	{
		return (this->*mCollisionChecks[funcIndex])(_obj1, _obj2, _collisionInfo);
	}

	return false;
}

void PhysicsManager::RegisterCollisionChecks()
{
	//			PLANE	CIRCLE	BOX
	// PLANE
	// CIRCLE
	// BOX

	mCollisionChecks = {
		&PhysicsManager::Plane2Plane,	&PhysicsManager::Circle2Plane,	&PhysicsManager::Box2Plane,
		&PhysicsManager::Plane2Circle,	&PhysicsManager::Circle2Circle,	&PhysicsManager::Box2Circle,
		&PhysicsManager::Plane2Box,		&PhysicsManager::Circle2Box,	&PhysicsManager::Box2Box,
	};
}

#pragma region Collision Checks
bool PhysicsManager::Plane2Plane(PhysicsObject* _plane1, PhysicsObject* _plane2, CollisionInfo& _collisionInfo)
{
	return false;
}

bool PhysicsManager::Circle2Plane(PhysicsObject* _circle, PhysicsObject* _plane, CollisionInfo& _collisionInfo)
{
	CircleCollider* circle = static_cast<CircleCollider*>(_circle);
	PlaneCollider* plane = static_cast<PlaneCollider*>(_plane);

	Vector2D planeToCircle = circle->GetPosition() - plane->GetPosition();

	float distFromSurface = std::abs(Vector2D::Dot(planeToCircle, plane->GetNormal()));
	float velDirection = Vector2D::Dot(circle->GetVelocity(), plane->GetNormal());

	// Circle is within collision distance of infinite planes' surface
	// and moving towards plane's surface
	if (distFromSurface <= circle->GetRadius() && velDirection < 0.f)
	{
		// Use a referece point on the plane to determine if circle is within plane bounds
		Vector2D pointOnPlane = P2D_Maths::ProjectPointOnPlane(circle->GetPosition(), plane->GetPosition(), plane->GetNormal());
		float distFromCenter = Vector2D::Distance(plane->GetPosition(), pointOnPlane) - circle->GetRadius();

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

bool PhysicsManager::Box2Plane(PhysicsObject* _box, PhysicsObject* _plane, CollisionInfo& _collisionInfo)
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

bool PhysicsManager::Plane2Circle(PhysicsObject* _plane, PhysicsObject* _circle, CollisionInfo& _collisionInfo)
{
	return Circle2Plane(_circle, _plane, _collisionInfo);
}

bool PhysicsManager::Circle2Circle(PhysicsObject* _circle1, PhysicsObject* _circle2, CollisionInfo& _collisionInfo)
{
	CircleCollider* circle1 = static_cast<CircleCollider*>(_circle1);
	CircleCollider* circle2 = static_cast<CircleCollider*>(_circle2);

	Vector2D toOtherCircle = circle2->GetPosition() - circle1->GetPosition();

	float dist = toOtherCircle.AbsMagnitude();
	float combRadius = circle2->GetRadius() + circle1->GetRadius();

	if (dist <= combRadius) // the circles are colliding
	{
		float penetration = combRadius - dist;
		Vector2D normal = toOtherCircle.Normalize();

		_collisionInfo.contactPoint = circle1->GetPosition() + normal * circle1->GetRadius();
		_collisionInfo.penetration = penetration;
		_collisionInfo.normal = normal;

		circle1->ResolveCollision(circle2, _collisionInfo);

		return true;
	}

	return false;
}

bool PhysicsManager::Box2Circle(PhysicsObject* _box, PhysicsObject* _circle, CollisionInfo& _collisionInfo)
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

bool PhysicsManager::Plane2Box(PhysicsObject* _plane, PhysicsObject* _box, CollisionInfo& _collisionInfo)
{
	return Box2Plane(_box, _plane, _collisionInfo);
}

bool PhysicsManager::Circle2Box(PhysicsObject* _circle, PhysicsObject* _box, CollisionInfo& _collisionInfo)
{
	return Box2Circle(_box, _circle, _collisionInfo);
}

bool PhysicsManager::Box2Box(PhysicsObject* _box1, PhysicsObject* _box2, CollisionInfo& _collisionInfo)
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
