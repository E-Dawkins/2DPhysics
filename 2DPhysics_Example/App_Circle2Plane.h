#pragma once
#include "App.h"

#include <CircleCollider.h>
#include <PlaneCollider.h>

class App_Circle2Plane : public App
{
public:
	virtual void Startup() override
	{
		CircleCollider* circle = new CircleCollider({ -12, 0 }, 1.f, 1.f);

		PlaneCollider* wallL = new PlaneCollider({ -18,  0 }, 1.f, 10.f, 270.f);
		PlaneCollider* wallR = new PlaneCollider({  18,  0 }, 1.f, 10.f, 90.f);
		PlaneCollider* wallB = new PlaneCollider({  0, -10 }, 1.f, 18.f, 0.f);
		PlaneCollider* wallT = new PlaneCollider({  0,  10 }, 1.f, 18.f, 180.f);

		circle->SetVelocity(Vector2D::AngleToUnitVector(125) * 8);

		AddObject(circle, BLUE);
		AddObject(wallL, YELLOW);
		AddObject(wallR, YELLOW);
		AddObject(wallB, YELLOW);
		AddObject(wallT, YELLOW);
	}
};