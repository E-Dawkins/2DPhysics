#include "raylib.h"
#include "Vector2D.h"

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
    static Vector2D pos(10, 20);
    pos.X += _deltaSeconds * 5;
    pos.Y += _deltaSeconds * 10;

    std::cout << pos.X << ", " << pos.Y << std::endl;
}

void Draw()
{
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
}