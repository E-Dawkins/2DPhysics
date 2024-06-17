#include "raylib.h"
#include "CircleCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"

#include <iostream>

#pragma region Variables
const int SCREEN_W = 960;
const int SCREEN_H = 540;
const Color BG_COLOR = { 50, 50, 50, 255 };

Camera2D camera;
RenderTexture2D target;

BoxCollider* box;
CircleCollider* circle;

PlaneCollider* wallL;
PlaneCollider* wallR;
PlaneCollider* wallT;
PlaneCollider* wallB;
#pragma endregion

#pragma region FunctionDeclarations
void Begin();
void Update(float _deltaSeconds);
void Draw();

Vector2 ConvertVector2D(Vector2D _vec);

void DrawCircleObject(CircleCollider* _circle, Color _color);
void DrawPlaneObject(PlaneCollider* _plane, Color _color);
void DrawBoxObject(BoxCollider* _box, Color _color);

void StartPhysicsSim();
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
    camera = { 0 };
    camera.zoom = 25;
    camera.target = { 0, 0 };
    camera.offset = { SCREEN_W / 2, SCREEN_H / 2 };

    target = LoadRenderTexture(SCREEN_W, SCREEN_H);

    box = new BoxCollider({ 0, 0 }, 1.f, { 1.f, 1.f }, 40.f);
    circle = new CircleCollider({ 1, 5 }, 1.f, 1.f);

    wallB = new PlaneCollider({ 0, -9 }, 1.f, 20.f, 0.f);
    wallT = new PlaneCollider({ 0,  9 }, 1.f, 20.f, 180.f);
    wallL = new PlaneCollider({-18, 0 }, 1.f, 20.f, 90.f);
    wallR = new PlaneCollider({ 18, 0 }, 1.f, 20.f, 270.f);
}

void Update(float _deltaSeconds)
{
    box->Update(_deltaSeconds);
    circle->Update(_deltaSeconds);

    StartPhysicsSim();

    CollisionInfo collInfo = {};
    if (box->CheckCollision(wallB, collInfo))
    {
        wallB->ResolveCollision(box, collInfo);
    }
    collInfo = {};
    if (box->CheckCollision(wallT, collInfo))
    {
        wallT->ResolveCollision(box, collInfo);
    }
    collInfo = {};
    if (box->CheckCollision(circle, collInfo))
    {
        box->ResolveCollision(circle, collInfo);
    }
    collInfo = {};
    if (circle->CheckCollision(wallB, collInfo))
    {
        wallB->ResolveCollision(circle, collInfo);
    }
    collInfo = {};
    if (circle->CheckCollision(wallT, collInfo))
    {
        wallT->ResolveCollision(circle, collInfo);
    }
}

void Draw()
{
    // Raylib has (0,0) top-left, so to make it intuitive to understand this
    // chain of Begin commands essentially flips the coordinate system vertically.

    /* [==== I.e. (0,0) is now bottom-left, +Y is up and -Y is down. ====] */

    BeginDrawing();
    {
        BeginTextureMode(target);
        {
            ClearBackground(BG_COLOR);

            BeginMode2D(camera);
            {
                DrawBoxObject(box, RED);
                DrawCircleObject(circle, BLUE);

                DrawPlaneObject(wallB, YELLOW);
                DrawPlaneObject(wallT, YELLOW);
            }
            EndMode2D();
        }
        EndTextureMode();

        DrawTexture(target.texture, 0, 0, WHITE);
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
    Vector2D pos = _circle->GetPosition();
    Vector2D up = _circle->GetLocalUp();

    Rectangle rect = { pos.X, pos.Y, radius * 0.2f, radius };

    DrawCircleV({ rect.x, rect.y }, radius, _color);
    DrawRectanglePro(rect, { rect.width / 2, 0 }, -_circle->GetRotationDegrees(), BLACK);
}

void DrawPlaneObject(PlaneCollider* _plane, Color _color)
{
    float halfExtent = _plane->GetHalfExtent();
    Vector2D pos = _plane->GetPosition();
    Vector2D right = _plane->GetLocalRight();

    Rectangle rect = { pos.X, pos.Y, 0.2f, halfExtent * 2.f };
    Rectangle rect2 = { pos.X, pos.Y, 0.2f, 1.f };

    DrawRectanglePro(rect2, { rect2.width / 2, 0 }, -_plane->GetRotationDegrees(), BLACK);
    DrawRectanglePro(rect, { rect.width / 2, rect.height / 2 }, -_plane->GetRotationDegrees() + 90.f, _color);
}

void DrawBoxObject(BoxCollider* _box, Color _color)
{
    Vector2D position = _box->GetPosition();
    Vector2D halfExtents = _box->GetHalfExtents();
    Vector2D up = _box->GetLocalUp();

    Rectangle rect  = { position.X, position.Y, halfExtents.X * 2.f, halfExtents.Y * 2.f };
    Rectangle rect2 = { position.X, position.Y, halfExtents.X * 0.2f, halfExtents.Y };

    const auto points = _box->GetPoints();

    DrawLineEx(ConvertVector2D(points[0]), ConvertVector2D(points[1]), 0.1f, _color);
    DrawLineEx(ConvertVector2D(points[1]), ConvertVector2D(points[3]), 0.1f, _color);
    DrawLineEx(ConvertVector2D(points[2]), ConvertVector2D(points[0]), 0.1f, _color);
    DrawLineEx(ConvertVector2D(points[3]), ConvertVector2D(points[2]), 0.1f, _color);
}

void StartPhysicsSim()
{
    static bool started = false;

    if (started)
        return;

    started = true;

    //box->SetVelocity(Vector2D::AngleToUnitVector(30.f) * 5.f);
    box->SetVelocity({0, -3});
}
#pragma endregion