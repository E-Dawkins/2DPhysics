#pragma once
#include "App.h"

#include "CircleCollider.h"

class App_Circle2Circle : public App
{
public:
	virtual void Startup() override
	{
		App::Startup();

		CircleCollider* circle1 = new CircleCollider({ -5,  0.5 }, 1.f, 1.f);
		CircleCollider* circle2 = new CircleCollider({  5, -0.5 }, 1.f, 1.f);

		circle1->SetVelocity({  3, 0 });
		circle2->SetVelocity({ -3, 0 });
	}
};