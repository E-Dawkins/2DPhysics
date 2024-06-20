#include "App.h"

#include "CircleCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"

void App::Update(const float _deltaSeconds)
{
	for (auto obj : mObjects)
	{
		obj.first->Update(_deltaSeconds);

		for (auto obj2 : mObjects)
		{
			if (obj.first != obj2.first)
			{
				CollisionInfo collInfo = {};
				obj.first->CheckCollision(obj2.first, collInfo);
			}
		}
	}
}

void App::Draw()
{
	for (auto obj : mObjects)
	{
		switch (obj.first->GetColliderType())
		{
			case CIRCLE: DrawCircleObject(obj.first, obj.second); break;
			case PLANE: DrawPlaneObject(obj.first, obj.second); break;
			case BOX: DrawPlaneObject(obj.first, obj.second); break;

			default: {} break;
		}
	}
}

void App::AddObject(PhysicsObject* _obj, Color _color)
{
	mObjects.insert({ _obj, _color });
}

Vector2 App::ConvertVector2D(Vector2D _vec)
{
	return { _vec.X, _vec.Y };
}

void App::DrawCircleObject(PhysicsObject* _circle, Color _color)
{
	CircleCollider* circle = static_cast<CircleCollider*>(_circle);

	float radius = circle->GetRadius();
	Vector2D pos = circle->GetPosition();
	Vector2D up = circle->GetLocalUp();

	Rectangle rect = { pos.X, pos.Y, radius * 0.2f, radius };

	DrawCircleV({ rect.x, rect.y }, radius, _color);
	DrawRectanglePro(rect, { rect.width / 2, 0 }, -circle->GetRotationDegrees(), BLACK);
}

void App::DrawPlaneObject(PhysicsObject* _plane, Color _color)
{
	PlaneCollider* plane = static_cast<PlaneCollider*>(_plane);

	float halfExtent = plane->GetHalfExtent();
	Vector2D pos = plane->GetPosition();
	Vector2D right = plane->GetLocalRight();

	Rectangle rect = { pos.X, pos.Y, 0.2f, halfExtent * 2.f };
	Rectangle rect2 = { pos.X, pos.Y, 0.2f, 1.f };

	DrawRectanglePro(rect2, { rect2.width / 2, 0 }, -plane->GetRotationDegrees(), BLACK);
	DrawRectanglePro(rect, { rect.width / 2, rect.height / 2 }, -plane->GetRotationDegrees() + 90.f, _color);
}

void App::DrawBoxObject(PhysicsObject* _box, Color _color)
{
	BoxCollider* box = static_cast<BoxCollider*>(_box);

	Vector2D position = box->GetPosition();
	Vector2D halfExtents = box->GetHalfExtents();
	Vector2D up = box->GetLocalUp();

	Rectangle rect = { position.X, position.Y, halfExtents.X * 2.f, halfExtents.Y * 2.f };
	Rectangle rect2 = { position.X, position.Y, halfExtents.X * 0.2f, halfExtents.Y };

	const auto points = box->GetPoints();

	DrawLineEx(ConvertVector2D(points[0]), ConvertVector2D(points[1]), 0.1f, _color);
	DrawLineEx(ConvertVector2D(points[1]), ConvertVector2D(points[3]), 0.1f, _color);
	DrawLineEx(ConvertVector2D(points[2]), ConvertVector2D(points[0]), 0.1f, _color);
	DrawLineEx(ConvertVector2D(points[3]), ConvertVector2D(points[2]), 0.1f, _color);
}
