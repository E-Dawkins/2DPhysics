#pragma once
#include "App.h"

#include <BoxCollider.h>
#include <CircleCollider.h>

class App_Box2Circle : public App
{
public:
	virtual void Startup() override
	{
		BoxCollider* box = new BoxCollider({ 5, 0 }, 1, { 1, 3 }, 0);
		CircleCollider* circle = new CircleCollider({ -5, 2 }, 1, 1);

		circle->SetVelocity({ 5, 0 });

		AddObject(box, RED);
		AddObject(circle, BLUE);
	}
};