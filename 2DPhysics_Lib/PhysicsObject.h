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

struct PHYSICS_API CollisionInfo
{
	CollisionInfo()
	{
		contactPoint = Vector2D(0, 0);
		penetration = FLT_MAX;
		normal = Vector2D(0, 0);
	}

	Vector2D contactPoint;
	float penetration;
	Vector2D normal;
};

const float MIN_LINEAR_THRESHOLD = 0.001f;
const float MIN_ANGULAR_THRESHOLD = 0.001f;

class PHYSICS_API PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(Vector2D _position, float _mass, float _rotation = 0.f);

	void Update(float _deltaSeconds);

	virtual void ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);
	void ApplyForce(Vector2D _force, const Vector2D _contact = Vector2D(0, 0));

	// Getters
	const Vector2D GetPosition() const				{ return mPosition; }
	const Vector2D GetVelocity() const				{ return mVelocity; }
	const Vector2D GetLocalRight() const			{ return mLocalRight; }
	const Vector2D GetLocalUp() const				{ return mLocalUp; }
	const Vector2D GetLocalGravity() const			{ return mLocalGravity; }
	const float GetMass() const						{ return mKinematic ? INT_MAX : mMass; }
	const float GetRotation() const					{ return mRotation; }
	const float GetRotationDegrees() const			{ return P2D_Maths::Rad2Deg(mRotation); }
	const float GetElasticity() const				{ return mElasticity; }
	const float GetMoment() const					{ return mKinematic ? INT_MAX : mMoment; }
	const float GetAngularVelocity() const			{ return mAngularVelocity; }
	const float GetLinearDrag() const				{ return mLinearDrag; }
	const float GetAngularDrag() const				{ return mAngularDrag; }
	const bool IsKinematic() const					{ return mKinematic; }
	const bool IsUsingGravity() const				{ return mUseGravity; }
	const ColliderType GetColliderType() const		{ return mColliderType; }

	// Setters
	void SetPosition(const Vector2D _position)				{ mPosition = _position; }
	void SetVelocity(const Vector2D _velocity)				{ mVelocity = _velocity; }
	void SetLocalGravity(const Vector2D _localGravity)		{ mLocalGravity = _localGravity; }
	void SetMass(const float _mass)							{ mMass = _mass; }
	void SetRotationDegrees(const float _rotation);
	void SetElasticity(const float _elasticity)				{ mElasticity = _elasticity; }
	void SetMoment(const float _moment)						{ mMoment = _moment; }
	void SetAngularVelocity(const float _angularVelocity)	{ mAngularVelocity = _angularVelocity; }
	void SetLinearDrag(const float _linearDrag)				{ mLinearDrag = _linearDrag; }
	void SetAngularDrag(const float _angularDrag)			{ mAngularDrag = _angularDrag; }
	void SetKinematic(const bool _kinematic)				{ mKinematic = _kinematic; }
	void SetUseGravity(const bool _useGravity)				{ mUseGravity = _useGravity; }

protected:
	void ApplyContactForces(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);

	void UpdateLocalAxes();

protected:
	ColliderType mColliderType;

	Vector2D mPosition;
	Vector2D mVelocity;
	Vector2D mLocalRight;
	Vector2D mLocalUp;
	Vector2D mLocalGravity;

	float mMass;
	float mRotation;
	float mLastRotation;
	float mElasticity;
	float mAngularVelocity;
	float mMoment;
	float mLinearDrag;
	float mAngularDrag;
	
	bool mKinematic;
	bool mUseGravity;
};

