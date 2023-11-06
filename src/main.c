#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

#define screenWidth 800
#define screenHeight 600
#define radius 25

#define verticalSpacing 100
#define textsize 20

void drawNode(node* root, int x, int y, int level) {
    if (root != NULL) {
        int dynamicSpacing = screenWidth / (1 << (level+2));

        if(root->leaf==true) {
            DrawCircleLines(x, y, radius, RED);
            DrawText(TextFormat("%c", root->c), x - 10, y - 10, textsize, PINK);
        } else {
            DrawCircleLines(x, y, radius, YELLOW);
            DrawText(TextFormat("%d", root->freq), x - 10, y - 10, textsize, WHITE);
        }

        if (root->left != NULL) {
            DrawLine(x - dynamicSpacing, y + verticalSpacing, x, y, GREEN);
            drawNode(root->left, x - dynamicSpacing, y + verticalSpacing, level + 1);
        }

        if (root->right != NULL) {
            DrawLine(x + dynamicSpacing, y + verticalSpacing, x, y, GREEN);
            drawNode(root->right, x + dynamicSpacing, y + verticalSpacing, level + 1);
        }
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "huft");

    Camera2D camera = { 0 };
    camera.target = (Vector2) {
        0.0f, 0.0f
    };

    camera.offset = (Vector2) {
        screenWidth / -10.0f, screenHeight / 10.0f
    };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Vector2 mousePosition = { 0.0f, 0.0f };
    Vector2 previousMousePosition = { 0.0f, 0.0f };

    SetTargetFPS(60);

    char ar[] = "nialksjdflsdjfkldsjfldsjflksdjlfkjdl";
    tree *t = treeInit(ar);
    while(1) {
        if(treeStateNext(t) == 1) break;
    }

    while (!WindowShouldClose()) {
        previousMousePosition = mousePosition;
        mousePosition = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        drawNode(t->root, screenWidth / 2, 50, 0);

        EndMode2D();
        EndDrawing();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            camera.target.x -= (mousePosition.x - previousMousePosition.x);
            camera.target.y -= (mousePosition.y - previousMousePosition.y);
        }
    }

    freeTree(t);
    CloseWindow();
    return 0;
}