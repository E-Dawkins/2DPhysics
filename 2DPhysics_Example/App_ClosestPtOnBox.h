#pragma once
#include "App.h"

#include <BoxCollider.h>

class App_ClosestPtOnBox : public App
{
protected:
	Vector2D pt;
	Vector2D projectedPt;

	BoxCollider* box;

public:
	virtual void Startup() override
	{
		box = new BoxCollider({ 0, 0 }, 1, { 1, 1 });
		AddObject(box, ORANGE);
	}

	virtual void Update(const float _deltaSeconds)
	{
		App::Update(_deltaSeconds);

		static float time = 0.f;
		time += _deltaSeconds;

		pt = Vector2D(std::sinf(time), std::cosf(time)) * 3.f;
		projectedPt = P2D_Maths::ClosestPointOnBox(pt, box->GetPosition(), box->GetHalfExtents(), box->GetLocalUp(), box->GetLocalRight());
	}

	virtual void Draw()
	{
		App::Draw();

		DrawCircleV(ConvertVector2D(pt), 0.25f, GREEN);
		DrawCircleV(ConvertVector2D(projectedPt), 0.25f, ORANGE);
	}
};