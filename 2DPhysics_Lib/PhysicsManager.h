#pragma once
#include "DLLCommon.h"

#include <vector>
#include <thread>

class PhysicsObject;

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

private:
	static PhysicsManager* mInstance;

	std::thread mPhysicsThread;

	std::vector<PhysicsObject*> mObjects;

	int mTickRate;
	float mDeltaSeconds;
	
	bool mQuit;
};
