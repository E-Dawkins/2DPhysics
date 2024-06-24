#pragma once
#include "App.h"

#include <PlaneCollider.h>
#include <CircleCollider.h>
#include <BoxCollider.h>

class App_AllPhysicsObjs : public App
{
protected:
	bool started = false;

public:
	virtual void Startup() override
	{
		PlaneCollider* plane1 = new PlaneCollider({  10, -5 }, 0, 12,  25);
		PlaneCollider* plane2 = new PlaneCollider({ -10, -5 }, 0, 12, -25);

		CircleCollider* peg1 = new CircleCollider({  0, -2 }, 0, 0.25f);
		CircleCollider* peg2 = new CircleCollider({ -5, -2 }, 0, 0.25f);
		CircleCollider* peg3 = new CircleCollider({  5, -2 }, 0, 0.25f);
		CircleCollider* peg4 = new CircleCollider({  2, -5 }, 0, 0.25f);
		CircleCollider* peg5 = new CircleCollider({ -2, -5 }, 0, 0.25f);

		peg1->SetKinematic(true);
		peg2->SetKinematic(true);
		peg3->SetKinematic(true);
		peg4->SetKinematic(true);
		peg5->SetKinematic(true);

		BoxCollider* box1 = new BoxCollider({ 8, 6 }, 1, { 1, 1 });
		BoxCollider* box2 = new BoxCollider({ 1, 6 }, 1, { 1, 1 });
		BoxCollider* box3 = new BoxCollider({ 2, 9 }, 1, { 1, 1 });

		CircleCollider* circle1 = new CircleCollider({ -6, 5 }, 1, 2);

		AddObject(plane1, YELLOW);
		AddObject(plane2, YELLOW);
		AddObject(peg1, BLUE);
		AddObject(peg2, BLUE);
		AddObject(peg3, BLUE);
		AddObject(peg4, BLUE);
		AddObject(peg5, BLUE);
		AddObject(box1, RED);
		AddObject(box2, RED);
		AddObject(box3, RED);
		AddObject(circle1, RED);

		for (auto& objPair : mObjects)
		{
			if (!objPair.object->IsKinematic())
			{
				objPair.object->SetUseGravity(true);
				objPair.object->SetLocalGravity({ 0, -9.8f });
				objPair.object->SetElasticity(0.5f);
			}
		}
	}

	virtual void Update(float _deltaSeconds) override
	{
		if (started)
		{
			App::Update(_deltaSeconds);
		}
		else
		{
			started = (GetKeyPressed() == KEY_SPACE);
		}
	}
};