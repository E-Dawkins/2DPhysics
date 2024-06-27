#include "PhysicsManager.h"
#include "PhysicsObject.h"

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
			if (object1->CheckCollision(object2, collInfo))
			{
				// TODO
			}
		}
	}
}
