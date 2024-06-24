#pragma once
#include "App.h"

#include <BoxCollider.h>
#include <PlaneCollider.h>

#include <iostream>

class App_Box2Plane : public App
{
public:
	virtual void Startup() override
	{
		BoxCollider* box = new BoxCollider({ 0, 0 }, 1, { 1, 1 }, 50);
		PlaneCollider* plane = new PlaneCollider({ 0, -10 }, 1, 18);

		box->SetUseGravity(true);
		box->SetLocalGravity({ 0, -9.8f });
		box->SetElasticity(0.3f);

		AddObject(box, RED);
		AddObject(plane, YELLOW);
	}
};