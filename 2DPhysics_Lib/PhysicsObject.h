#pragma once
#include "Vector2D.h"
#include <vector>

#pragma warning (disable : 4251)

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
	virtual bool CheckCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo) = 0;
	void ResolveCollision(PhysicsObject* _otherObject, CollisionInfo& _collisionInfo);

	// Getters
	const Vector2D GetPosition() const				{ return mPosition; }
	const Vector2D GetVelocity() const				{ return mVelocity; }
	const float GetMass() const						{ return mMass; }
	const float GetRotation() const					{ return mRotation; }
	const float GetElasticity() const				{ return mElasticity; }

	// Setters
	void SetPosition(const Vector2D _position)	{ mPosition = _position; }
	void SetVelocity(const Vector2D _velocity)	{ mVelocity = _velocity; }
	void SetMass(const float _mass)				{ mMass = _mass; }
	void SetRotation(const float _rotation)		{ mRotation = _rotation; }
	void SetElasticity(const float _elasticity) { mElasticity = _elasticity; }

protected:
	Vector2D mPosition;
	Vector2D mVelocity;
	float mMass;
	float mRotation;
	float mElasticity;
};

