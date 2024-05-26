#pragma once
#include "Vector2D.h"
#include <vector>

struct PHYSICS_API CollisionInfo
{
	CollisionInfo()
	{
		collisionPoints = {};
		minPenetration = 0.f;
	}

	std::vector<Vector2D> collisionPoints;
	float minPenetration;
};

class PHYSICS_API PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(Vector2D _position, float _mass, float _rotation = 0.f);

	void Update(float _deltaSeconds);
	virtual bool CheckCollision(PhysicsObject* _otherObject) = 0;
	void ResolveCollision(PhysicsObject* _otherObject);

	// Getters
	const Vector2D GetPosition() const				{ return mPosition; }
	const Vector2D GetVelocity() const				{ return mVelocity; }
	const float GetMass() const						{ return mMass; }
	const float GetRotation() const					{ return mRotation; }
	const CollisionInfo GetCollisionInfo() const	{ return mCollisionInfo; }

	// Setters
	void SetPosition(const Vector2D _position)	{ mPosition = _position; }
	void SetVelocity(const Vector2D _velocity)	{ mVelocity = _velocity; }
	void SetMass(const float _mass)				{ mMass = _mass; }
	void SetRotation(const float _rotation)		{ mRotation = _rotation; }

protected:
	Vector2D mPosition;
	Vector2D mVelocity;
	float mMass;
	float mRotation;

	CollisionInfo mCollisionInfo;
};

