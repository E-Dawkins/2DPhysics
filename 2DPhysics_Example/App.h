#pragma once
#include <vector>
#include <raylib.h>

#include "PhysicsObject.h"
#include <PhysicsManager.h>

class App
{
public:
	virtual void Startup();
	virtual void Update(const float _deltaSeconds);
	virtual void Draw();

protected:
	Vector2 ConvertVector2D(Vector2D _vec);

	void DrawCircleObject(PhysicsObject* _circle, Color _color);
	void DrawPlaneObject(PhysicsObject* _plane, Color _color);
	void DrawBoxObject(PhysicsObject* _box, Color _color);
};

