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
		App::Startup();

		box = new BoxCollider({ 0, 0 }, 1, { 2, 3 });
	}

	virtual void Update(const float _deltaSeconds)
	{
		App::Update(_deltaSeconds);

		static float time = 0.f;
		time += _deltaSeconds;

		static float maxLength = std::max(box->GetHalfExtents().X, box->GetHalfExtents().Y);
		pt = Vector2D(std::sinf(time) * (maxLength * 2.f), std::cosf(time) * (maxLength * 2.f));
		projectedPt = P2D_Maths::ClosestPointOnBox(pt, box->GetPosition(), box->GetHalfExtents(), box->GetLocalUp(), box->GetLocalRight());

		box->SetRotationDegrees(time * 30.f);
	}

	virtual void Draw()
	{
		App::Draw();

		DrawCircleV(ConvertVector2D(pt), 0.25f, GREEN);
		DrawCircleV(ConvertVector2D(projectedPt), 0.25f, ORANGE);
	}
};