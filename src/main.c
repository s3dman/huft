#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "huft");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}