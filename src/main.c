#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "huft");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Congrats! You created your first window!", 190, 200, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}