#pragma once
#include "DLLCommon.h"

#include <vector>
#include <thread>

#include "PhysicsObject.h"

class PHYSICS_API PhysicsManager
{
public:
	static PhysicsManager& GetInstance()
	{
		if (!mInstance)
			mInstance = new PhysicsManager();

		return *mInstance;
	}

	void Init();
	void Quit();

	void RegisterObject(PhysicsObject* _object) { mObjects.push_back(_object); }

	// Get the amount of ticks-per-second.
	const int GetTickRate() const							{ return mTickRate; }
	const std::vector<PhysicsObject*>& GetAllObjects()		{ return mObjects; }

	// Set the amount of ticks-per-second.
	void SetTickRate(const int _tickRate) 
	{ 
		mTickRate = _tickRate;
		mDeltaSeconds = 1.f / (float)mTickRate;
	}

	// Delete the copy / assignment operator, to prevent external instantiation
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager& operator=(const PhysicsManager&) = delete;

private:
	// Private constructor / destructor to prevent external instantiation / deletion
	PhysicsManager();
	~PhysicsManager();

	void FixedUpdate();
	void UpdateObjs();
	void CheckCollisions();

	bool CheckCollision(PhysicsObject* _obj1, PhysicsObject* _obj2, CollisionInfo& _collisionInfo);
	void RegisterCollisionChecks();

	// Collision checks
	bool Plane2Plane(PhysicsObject* _plane1, PhysicsObject* _plane2, CollisionInfo& _collisionInfo);
	bool Circle2Plane(PhysicsObject* _circle, PhysicsObject* _plane, CollisionInfo& _collisionInfo);
	bool Box2Plane(PhysicsObject* _box, PhysicsObject* _plane, CollisionInfo& _collisionInfo);
	bool Plane2Circle(PhysicsObject* _plane, PhysicsObject* _circle, CollisionInfo& _collisionInfo);
	bool Circle2Circle(PhysicsObject* _circle1, PhysicsObject* _circle2, CollisionInfo& _collisionInfo);
	bool Box2Circle(PhysicsObject* _box, PhysicsObject* _circle, CollisionInfo& _collisionInfo);
	bool Plane2Box(PhysicsObject* _plane, PhysicsObject* _box, CollisionInfo& _collisionInfo);
	bool Circle2Box(PhysicsObject* _circle, PhysicsObject* _box, CollisionInfo& _collisionInfo);
	bool Box2Box(PhysicsObject* _box1, PhysicsObject* _box2, CollisionInfo& _collisionInfo);

private:
	static PhysicsManager* mInstance;

	typedef bool (PhysicsManager::* CollisionCheck)(PhysicsObject*, PhysicsObject*, CollisionInfo&);
	std::array<CollisionCheck, COLLIDER_TYPE_MAX * COLLIDER_TYPE_MAX> mCollisionChecks;

	std::thread mPhysicsThread;

	std::vector<PhysicsObject*> mObjects;

	int mTickRate;
	float mDeltaSeconds;
	
	bool mQuit;
};
