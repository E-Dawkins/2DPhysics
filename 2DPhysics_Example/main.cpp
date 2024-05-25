#include "raylib.h"
#include "2DPhysics_Lib.h"

void Draw();

int main()
{
    InitWindow(800, 500, "2D Physics Example");
    SetTargetFPS(60);

    PrintTest();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        Draw();

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

void Draw()
{
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
}