#include "raylib.h"
#include "PhysicsObject.h"

#include <iostream>

void Update(float _deltaSeconds);
void Draw();

int main()
{
    InitWindow(800, 500, "2D Physics Example");
    SetTargetFPS(60);

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

void Update(float _deltaSeconds)
{
    static PhysicsObject obj = PhysicsObject(Vector2D(10, 20), 10);
    obj.SetVelocity(Vector2D(5, 10));
    obj.Update(_deltaSeconds);

    std::cout << obj.GetPosition().X << ", " << obj.GetPosition().Y << std::endl;
}

void Draw()
{
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
}