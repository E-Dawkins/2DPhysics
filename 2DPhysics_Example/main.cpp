#include "raylib.h"
#include "CircleCollider.h"

#pragma region Variables
const int SCREEN_W = 960;
const int SCREEN_H = 540;

CircleCollider* circle1 = new CircleCollider(Vector2D(SCREEN_W/2, SCREEN_H/2), 1,       0.5f);
CircleCollider* circle2 = new CircleCollider(Vector2D(SCREEN_W/2, SCREEN_H/2), 20,    1.f);
CircleCollider* circle3 = new CircleCollider(Vector2D(SCREEN_W/2, SCREEN_H/2), 1,       0.5f);

Camera2D camera = { 0 };
const Color BG_COLOR = { 50, 50, 50, 255 };
#pragma endregion

#pragma region FunctionDeclarations
void Begin();
void Update(float _deltaSeconds);
void Draw();

Vector2 ConvertVector2D(Vector2D _vec);
void DrawCircleObject(CircleCollider* _circle, Color _color);
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
    circle1->SetVelocity(Vector2D(3, 0));

    circle3->SetPosition({ circle3->GetPosition().X - 10, circle3->GetPosition().Y - 0.2f });
    circle3->SetVelocity(Vector2D(3, 0));

    camera.zoom = 40;
    camera.offset = { SCREEN_W/2, SCREEN_H/2 };
    camera.target = { circle2->GetPosition().X, circle2->GetPosition().Y };
}

void Update(float _deltaSeconds)
{
    circle1->Update(_deltaSeconds);
    circle2->Update(_deltaSeconds);
    circle3->Update(_deltaSeconds);

    CollisionInfo collInfo;

    if (circle1->CheckCollision(circle2, collInfo))
    {
        circle1->ResolveCollision(circle2, collInfo);
    }

    if (circle1->CheckCollision(circle3, collInfo))
    {
        circle1->ResolveCollision(circle3, collInfo);
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
#pragma endregion