#include "raylib.h"
#include "CircleCollider.h"
#include "PlaneCollider.h"

#pragma region Variables
const int SCREEN_W = 960;
const int SCREEN_H = 540;

CircleCollider* circle1 = new CircleCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 1, 0.5f);
CircleCollider* circle2 = new CircleCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 5, 1.f);
CircleCollider* circle3 = new CircleCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 1, 0.5f);

PlaneCollider* plane1 = new PlaneCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 20, Vector2D(1, 0), 6.f);
PlaneCollider* plane2 = new PlaneCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 20, Vector2D(-1, 0), 6.f);
PlaneCollider* plane3 = new PlaneCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 20, Vector2D(0, 1), 10.f);
PlaneCollider* plane4 = new PlaneCollider(Vector2D(SCREEN_W / 2, SCREEN_H / 2), 20, Vector2D(0, -1), 10.f);

Camera2D camera = { 0 };
const Color BG_COLOR = { 50, 50, 50, 255 };
#pragma endregion

#pragma region FunctionDeclarations
void Begin();
void Update(float _deltaSeconds);
void Draw();

Vector2 ConvertVector2D(Vector2D _vec);

void DrawCircleObject(CircleCollider* _circle, Color _color);
void DrawPlaneObject(PlaneCollider* _plane, Color _color);
#pragma endregion

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "2D Physics Example");
    SetTargetFPS(60);

    Begin();

    while (!WindowShouldClose())
    {
        Update(GetFrameTime());
        Draw();
    }

    CloseWindow();
    
    return 0;
}

void Begin()
{
    circle1->SetPosition({circle1->GetPosition().X - 8, circle1->GetPosition().Y - 0.2f});
    circle1->SetVelocity(Vector2D(6, 0));

    circle3->SetPosition({ circle3->GetPosition().X - 10, circle3->GetPosition().Y - 0.3f });
    circle3->SetVelocity(Vector2D(3, 0));

    plane1->SetPosition({ plane1->GetPosition().X - 10, plane1->GetPosition().Y });
    plane2->SetPosition({ plane2->GetPosition().X + 10, plane2->GetPosition().Y });

    plane3->SetPosition({ plane3->GetPosition().X, plane3->GetPosition().Y - 6 });
    plane4->SetPosition({ plane4->GetPosition().X, plane4->GetPosition().Y + 6 });

    camera.zoom = 40;
    camera.offset = { SCREEN_W / 2, SCREEN_H / 2 };
    camera.target = { SCREEN_W / 2, SCREEN_H / 2 };
}

void Update(float _deltaSeconds)
{
    circle1->Update(_deltaSeconds);
    circle2->Update(_deltaSeconds);
    circle3->Update(_deltaSeconds);

    CollisionInfo collInfo;

    // CIRCLES
    if (circle1->CheckCollision(circle2, collInfo))
    {
        circle1->ResolveCollision(circle2, collInfo);
    }

    if (circle1->CheckCollision(circle3, collInfo))
    {
        circle1->ResolveCollision(circle3, collInfo);
    }

    if (circle2->CheckCollision(circle3, collInfo))
    {
        circle2->ResolveCollision(circle3, collInfo);
    }

    // PLANES
    // CIRCLE 1
    if (circle1->CheckCollision(plane1, collInfo))
    {
        plane1->ResolveCollision(circle1, collInfo);
    }

    if (circle1->CheckCollision(plane2, collInfo))
    {
        plane2->ResolveCollision(circle1, collInfo);
    }

    if (circle1->CheckCollision(plane3, collInfo))
    {
        plane3->ResolveCollision(circle1, collInfo);
    }

    if (circle1->CheckCollision(plane4, collInfo))
    {
        plane4->ResolveCollision(circle1, collInfo);
    }

    // CIRCLE 2
    if (circle2->CheckCollision(plane1, collInfo))
    {
        plane1->ResolveCollision(circle2, collInfo);
    }

    if (circle2->CheckCollision(plane2, collInfo))
    {
        plane2->ResolveCollision(circle2, collInfo);
    }

    if (circle2->CheckCollision(plane3, collInfo))
    {
        plane3->ResolveCollision(circle2, collInfo);
    }

    if (circle2->CheckCollision(plane4, collInfo))
    {
        plane4->ResolveCollision(circle2, collInfo);
    }

    // CIRCLE 3
    if (circle3->CheckCollision(plane1, collInfo))
    {
        plane1->ResolveCollision(circle3, collInfo);
    }

    if (circle3->CheckCollision(plane2, collInfo))
    {
        plane2->ResolveCollision(circle3, collInfo);
    }

    if (circle3->CheckCollision(plane3, collInfo))
    {
        plane3->ResolveCollision(circle3, collInfo);
    }

    if (circle3->CheckCollision(plane4, collInfo))
    {
        plane4->ResolveCollision(circle3, collInfo);
    }
}

void Draw()
{
    BeginDrawing();
    {
        ClearBackground(BG_COLOR);

        BeginMode2D(camera);
        {
            DrawCircleObject(circle1, RED);
            DrawCircleObject(circle2, SKYBLUE);
            DrawCircleObject(circle3, MAGENTA);

            DrawPlaneObject(plane1, YELLOW);
            DrawPlaneObject(plane2, YELLOW);
            DrawPlaneObject(plane3, YELLOW);
            DrawPlaneObject(plane4, YELLOW);
        }
        EndMode2D();
    }
    EndDrawing();
}

#pragma region Helpers
Vector2 ConvertVector2D(Vector2D _vec)
{
    return { _vec.X, _vec.Y };
}

void DrawCircleObject(CircleCollider* _circle, Color _color)
{
    float radius = _circle->GetRadius();
    Vector2 pos = ConvertVector2D(_circle->GetPosition());
    Vector2 facing = ConvertVector2D(-Vector2D::AngleToUnitVector(_circle->GetRotation()) * radius + _circle->GetPosition());

    DrawCircleV(pos, radius, _color);
    DrawLineEx(pos, facing, radius * 0.15f, BLACK);
}

void DrawPlaneObject(PlaneCollider* _plane, Color _color)
{
    float planeHalfExtent = _plane->GetHalfExtent();
    Vector2D planeCenter = _plane->GetPosition();
    Vector2D adjFacing = (Vector2D(_plane->GetNormal().Y, -_plane->GetNormal().X) * planeHalfExtent).Normalized();
    Vector2 start = ConvertVector2D(planeCenter - adjFacing * planeHalfExtent);
    Vector2 end = ConvertVector2D(planeCenter + adjFacing * planeHalfExtent);

    DrawLineEx(start, end, 0.1f, _color);
}
#pragma endregion