#include "raylib.h"
#include "CircleCollider.h"

#include <iostream>

void Begin();
void Update(float _deltaSeconds);
void Draw();

int main()
{
    InitWindow(800, 500, "2D Physics Example");
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

CircleCollider* circle1 = new CircleCollider(Vector2D(400, 0), 1, 25);
CircleCollider* circle2 = new CircleCollider(Vector2D(450, 200), 1, 50);

void Begin()
{
    circle1->SetVelocity(Vector2D(0, 100));
}

void Update(float _deltaSeconds)
{
    circle1->Update(_deltaSeconds);
    
    if (circle1->CheckCollision(circle2))
    {
        std::cout << "Colliding!" << std::endl;
    }
}

void Draw()
{
    ClearBackground(RAYWHITE);
    DrawCircle(circle1->GetPosition().X, circle1->GetPosition().Y, circle1->GetRadius(), RED);
    DrawCircle(circle2->GetPosition().X, circle2->GetPosition().Y, circle2->GetRadius(), SKYBLUE);
}