#pragma once
#include "Vector2D.h"

class PHYSICS_API PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(Vector2D _position, float _mass, float _rotation = 0.f);

	void Update(float _deltaSeconds);

	// Getters
	const Vector2D GetPosition() const		{ return mPosition; }
	const Vector2D GetVelocity() const		{ return mVelocity; }
	const float GetMass() const				{ return mMass; }
	const float GetRotation() const			{ return mRotation; }

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
};

