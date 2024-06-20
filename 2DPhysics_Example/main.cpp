#include "raylib.h"

#include "App.h"
#include "App_Circle2Circle.h"

#pragma region Variables
const int SCREEN_W = 960;
const int SCREEN_H = 540;
const Color BG_COLOR = { 50, 50, 50, 255 };

Camera2D camera;
RenderTexture2D target;

App* app;
#pragma endregion

#pragma region FunctionDeclarations
void Begin();
void Update(float _deltaSeconds);
void Draw();
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

    app = new App_Circle2Circle();
    app->Startup();
}

void Update(float _deltaSeconds)
{
    app->Update(_deltaSeconds);
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
                app->Draw();
            }
            EndMode2D();
        }
        EndTextureMode();

        DrawTexture(target.texture, 0, 0, WHITE);
    }
    EndDrawing();
}