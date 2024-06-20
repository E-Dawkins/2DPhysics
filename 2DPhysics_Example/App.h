#pragma once
#include <map>
#include <raylib.h>
#include "PhysicsObject.h"

class App
{
public:
	virtual void Startup() {}
	void Update(const float _deltaSeconds);
	void Draw();

protected:
	void AddObject(PhysicsObject* _obj, Color _color);
	Vector2 ConvertVector2D(Vector2D _vec);

	void DrawCircleObject(PhysicsObject* _circle, Color _color);
	void DrawPlaneObject(PhysicsObject* _plane, Color _color);
	void DrawBoxObject(PhysicsObject* _box, Color _color);

protected:
	std::map<PhysicsObject*, Color> mObjects;
};

