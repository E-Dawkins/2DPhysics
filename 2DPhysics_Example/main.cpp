#include "raylib.h"
#include "CircleCollider.h"

#include <iostream>

const int SCREEN_W = 800;
const int SCREEN_H = 500;
const int SCREEN_HW = SCREEN_W / 2;
const int SCREEN_HH = SCREEN_H / 2;

void Begin();
void Update(float _deltaSeconds);
void Draw();

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "2D Physics Example");
    SetTargetFPS(60);

    Begin();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        Update(GetFrameTime());
        Draw();

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

CircleCollider* circle1 = new CircleCollider(Vector2D(100, SCREEN_HH-50),       1,      25);
CircleCollider* circle2 = new CircleCollider(Vector2D(SCREEN_HW, SCREEN_HH),    10,     50);

void Begin()
{
    circle1->SetVelocity(Vector2D(100, 0));
}

void Update(float _deltaSeconds)
{
    circle1->Update(_deltaSeconds);
    circle2->Update(_deltaSeconds);
    
    CollisionInfo collInfo;

    if (circle1->CheckCollision(circle2, collInfo))
    {
        std::cout << "Colliding!" << std::endl;

        circle1->ResolveCollision(circle2, collInfo);
    }
}

void Draw()
{
    ClearBackground(RAYWHITE);
    DrawCircle((int)circle1->GetPosition().X, (int)circle1->GetPosition().Y, circle1->GetRadius(), RED);
    DrawCircle((int)circle2->GetPosition().X, (int)circle2->GetPosition().Y, circle2->GetRadius(), SKYBLUE);
}