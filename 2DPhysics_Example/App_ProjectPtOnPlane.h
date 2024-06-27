#pragma once
#include "App.h"

#include <PlaneCollider.h>

class App_ProjectPtOnPlane : public App
{
protected:
	Vector2D pt;
	Vector2D projectedPt;

	PlaneCollider* plane;

public:
	virtual void Startup() override
	{
		App::Startup();

		plane = new PlaneCollider({ 0, 0 }, 0, 5, 30);
	}

	virtual void Update(const float _deltaSeconds)
	{
		App::Update(_deltaSeconds);

		static float time = 0.f;
		time += _deltaSeconds;

		pt = Vector2D(std::sinf(time), std::cosf(time)) * 5.f;
		projectedPt = P2D_Maths::ProjectPointOnPlane(pt, plane->GetPosition(), plane->GetNormal());
	}

	virtual void Draw()
	{
		App::Draw();

		DrawCircleV(ConvertVector2D(pt), 0.25f, GREEN);
		DrawCircleV(ConvertVector2D(projectedPt), 0.25f, ORANGE);
	}
};