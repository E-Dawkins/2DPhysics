#pragma once
#include "App.h"

#include "BoxCollider.h"

class App_Box2Box : public App
{
public:
	virtual void Startup() override
	{
		App::Startup();

		BoxCollider* box1 = new BoxCollider({ -5,  1 }, 1.f, { 1, 1 }, 50);
		BoxCollider* box2 = new BoxCollider({  5,  0 }, 1.f, { 1, 1 });

		box1->SetVelocity({ 3, 0 });
	}
};