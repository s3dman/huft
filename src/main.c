#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

#define screenWidth 1280
#define screenHeight 720
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

    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Vector2 mousePosition = { 0.0f, 0.0f };
    Vector2 previousMousePosition = { 0.0f, 0.0f };

    SetTargetFPS(60);

    char ar[] = "hello niga";
    tree *t = treeInit(ar);

    bool nKeyPressed = false;
    while (!WindowShouldClose()) {
        previousMousePosition = mousePosition;
        mousePosition = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        drawNode(t->root, screenWidth / 2, 50, 0);
        for(int i=0; i<t->size; i++) {
            DrawText(TextFormat("f:%d-c:%c", t->nodes[i]->freq,t->nodes[i]->c),100*(i+1),screenHeight-50, textsize, WHITE);
        }

        EndMode2D();
        EndDrawing();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            camera.target.x -= (mousePosition.x - previousMousePosition.x);
            camera.target.y -= (mousePosition.y - previousMousePosition.y);
        }
        if (IsKeyDown(KEY_N) && !nKeyPressed) {
            if (treeStateNext(t) != 1) {
                // generateHuffmanCodes(t->root, "", 0);
                printf("\n");
            }
            nKeyPressed = true;
        }
        if (!IsKeyDown(KEY_N)) nKeyPressed = false;
    }

    freeTree(t);
    CloseWindow();
    return 0;
}