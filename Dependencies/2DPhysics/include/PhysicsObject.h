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

	COLLIDER_TYPE_MAX
};

#pragma warning (disable : 4251) // disables a warning on the std::vector in CollisionInfo

struct PHYSICS_API CollisionInfo
{
	CollisionInfo()
	{
		collisionPoints = {};
		penetration = 0.f;
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
	void ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);

	void ApplyForce(Vector2D _force, const Vector2D _contact = Vector2D(0, 0));
	void ApplyContactForces(PhysicsObject* _otherObject, Vector2D _normal, float _penetration);

	// Getters
	const Vector2D GetPosition() const				{ return mPosition; }
	const Vector2D GetVelocity() const				{ return mVelocity; }
	const float GetMass() const						{ return mMass; }
	const float GetRotation() const					{ return mRotation; }
	const float GetRotationDegrees() const			{ return mRotation * (180.f / Physics2D::pi); }
	const float GetElasticity() const				{ return mElasticity; }
	const float GetMoment() const					{ return mMoment; }

	// Setters
	void SetPosition(const Vector2D _position)	{ mPosition = _position; }
	void SetVelocity(const Vector2D _velocity)	{ mVelocity = _velocity; }
	void SetMass(const float _mass)				{ mMass = _mass; }
	void SetRotation(const float _rotation)		{ mRotation = _rotation; }
	void SetElasticity(const float _elasticity) { mElasticity = _elasticity; }
	void SetMoment(const float _moment)			{ mMoment = _moment; }

protected:
	void RegisterCollisionChecks();

	bool Circle2Circle(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo);
	bool Circle2Plane(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo);
	bool Plane2Circle(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo);
	bool Plane2Plane(PhysicsObject* _object1, PhysicsObject* _object2, CollisionInfo& _collisionInfo);

protected:
	typedef bool (PhysicsObject::* CollisionCheck)(PhysicsObject*, PhysicsObject*, CollisionInfo&);
	std::array<CollisionCheck, COLLIDER_TYPE_MAX * COLLIDER_TYPE_MAX> mCollisionChecks;

	ColliderType mColliderType;

	Vector2D mPosition;
	Vector2D mVelocity;
	float mMass;
	float mRotation;
	float mElasticity;
	float mAngularVelocity;
	float mMoment;
};

