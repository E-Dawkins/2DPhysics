#pragma once
#include "Vector2D.h"
#include <vector>
#include "Maths.h"
#include <array>

enum PHYSICS_API ColliderType
{
	UNKNOWN = -1,
	PLANE,
	CIRCLE,
	BOX,

	COLLIDER_TYPE_MAX
};

#pragma warning (disable : 4251) // disables a warning on the std::vector in CollisionInfo

struct PHYSICS_API CollisionInfo
{
	CollisionInfo()
	{
		collisionPoints = {};
		penetration = FLT_MAX;
		normal = Vector2D(0, 0);
	}

	std::vector<Vector2D> collisionPoints;
	float penetration;
	Vector2D normal;
};

class PHYSICS_API PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(Vector2D _position, float _mass, float _rotation = 0.f);

	void Update(float _deltaSeconds);
	bool CheckCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);
	virtual void ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);
	void ResolveCollision2(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);

	void ApplyForce(Vector2D _force, const Vector2D _contact = Vector2D(0, 0));
	void ApplyContactForces(PhysicsObject* _otherObject, Vector2D _normal, float _penetration);

	// Getters
	const Vector2D GetPosition() const				{ return mPosition; }
	const Vector2D GetVelocity() const				{ return mVelocity; }
	const Vector2D GetLocalRight() const			{ return mLocalRight; }
	const Vector2D GetLocalUp() const				{ return mLocalUp; }
	const float GetMass() const						{ return mKinematic ? FLT_MAX : mMass; }
	const float GetRotation() const					{ return mRotation; }
	const float GetRotationDegrees() const			{ return Physics2D::Rad2Deg(mRotation); }
	const float GetElasticity() const				{ return mElasticity; }
	const float GetMoment() const					{ return mKinematic ? FLT_MAX : mMoment; }
	const float GetAngularVelocity() const			{ return mAngularVelocity; }
	const bool IsKinematic() const					{ return mKinematic; }

	// Setters
	void SetPosition(const Vector2D _position)				{ mPosition = _position; }
	void SetVelocity(const Vector2D _velocity)				{ mVelocity = _velocity; }
	void SetMass(const float _mass)							{ mMass = _mass; }
	void SetRotationDegrees(const float _rotation);
	void SetElasticity(const float _elasticity)				{ mElasticity = _elasticity; }
	void SetMoment(const float _moment)						{ mMoment = _moment; }
	void SetAngularVelocity(const float _angularVelocity)	{ mAngularVelocity = _angularVelocity; }
	void SetKinematic(const bool _kinematic)				{ mKinematic = _kinematic; }

protected:
	void UpdateLocalAxes();
	void RegisterCollisionChecks();

	bool Plane2Plane(PhysicsObject* _plane1, PhysicsObject* _plane2, CollisionInfo& _collisionInfo);
	bool Circle2Plane(PhysicsObject* _circle, PhysicsObject* _plane, CollisionInfo& _collisionInfo);
	bool Box2Plane(PhysicsObject* _box, PhysicsObject* _plane, CollisionInfo& _collisionInfo);
	bool Plane2Circle(PhysicsObject* _plane, PhysicsObject* _circle, CollisionInfo& _collisionInfo);
	bool Circle2Circle(PhysicsObject* _circle1, PhysicsObject* _circle2, CollisionInfo& _collisionInfo);
	bool Box2Circle(PhysicsObject* _box, PhysicsObject* _circle, CollisionInfo& _collisionInfo);
	bool Plane2Box(PhysicsObject* _plane, PhysicsObject* _box, CollisionInfo& _collisionInfo);
	bool Circle2Box(PhysicsObject* _circle, PhysicsObject* _box, CollisionInfo& _collisionInfo);
	bool Box2Box(PhysicsObject* _box1, PhysicsObject* _box2, CollisionInfo& _collisionInfo);

protected:
	typedef bool (PhysicsObject::* CollisionCheck)(PhysicsObject*, PhysicsObject*, CollisionInfo&);
	std::array<CollisionCheck, COLLIDER_TYPE_MAX * COLLIDER_TYPE_MAX> mCollisionChecks;

	ColliderType mColliderType;

	Vector2D mPosition;
	Vector2D mVelocity;
	Vector2D mLocalRight;
	Vector2D mLocalUp;

	float mMass;
	float mRotation;
	float mLastRotation;
	float mElasticity;
	float mAngularVelocity;
	float mMoment;
	
	bool mKinematic;
};

