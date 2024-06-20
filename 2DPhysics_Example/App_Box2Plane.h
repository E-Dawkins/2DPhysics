#pragma once
#include "App.h"

#include <BoxCollider.h>
#include <PlaneCollider.h>

class App_Box2Plane : public App
{
public:
	virtual void Startup() override
	{
		BoxCollider* box = new BoxCollider({ 0, 0 }, 1, { 1, 1 }, 50);
		PlaneCollider* wallB = new PlaneCollider({ 0, -10 }, 1, 18);
		PlaneCollider* wallT = new PlaneCollider({ 0,  10 }, 1, 18, 180);

		box->SetVelocity({ 0, -3 });

		AddObject(box, RED);
		AddObject(wallB, YELLOW);
		AddObject(wallT, YELLOW);
	}
};