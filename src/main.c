#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

#define screenWidth 1280
#define screenHeight 720
#define radius 25

#define verticalSpacing 100
#define textsize 20

void nodeOverlay(int x,int y,char c, unsigned int f,Vector2 rpos) {
    if(CheckCollisionPointCircle(rpos,(Vector2) {
    x,y
},radius))
    DrawRectangle(100,100,100,100, GetColor(0xffffff33) );

}

void drawNode(node* root, int x, int y, int level,Vector2 rpos) {
    if (root != NULL) {
        int dynamicSpacing = screenWidth / (1 << (level+2));

        nodeOverlay(x,y,root->c,root->freq,rpos);
        if(root->leaf==true) {
            DrawCircleLines(x, y, radius, RED);
            DrawText(TextFormat("%c", root->c), x - 10, y - 10, textsize, PINK);
        } else {
            DrawCircleLines(x, y, radius, YELLOW);
            DrawText(TextFormat("%d", root->freq), x - 10, y - 10, textsize, WHITE);
        }

        if (root->left != NULL) {
            DrawLine(x - dynamicSpacing, y + verticalSpacing, x, y, GREEN);
            drawNode(root->left, x - dynamicSpacing, y + verticalSpacing, level + 1,rpos);
        }

        if (root->right != NULL) {
            DrawLine(x + dynamicSpacing, y + verticalSpacing, x, y, GREEN);
            drawNode(root->right, x + dynamicSpacing, y + verticalSpacing, level + 1,rpos);
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
    Font jbmTtf = LoadFontEx("../assets/JetBrainsMono-Regular.ttf", 32, 0, 0);

    while (!WindowShouldClose()) {
        previousMousePosition = mousePosition;
        mousePosition = GetMousePosition();
        Vector2 worldMousePos = GetScreenToWorld2D(mousePosition, camera);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        drawNode(t->root, screenWidth / 2, 50, 0,worldMousePos);
        for(int i=0; i<t->size; i++) {
            const char* a = TextFormat("{f:%d-c:%c}", t->nodes[i]->freq,t->nodes[i]->c);
            if(!t->nodes[i]->leaf)
                a = TextFormat("{f:%d}", t->nodes[i]->freq);
            DrawTextEx(jbmTtf,a, (Vector2) {
                0,32*i
            },32.0f,3, WHITE);
        }

        EndMode2D();
        EndDrawing();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            camera.target.x -= (mousePosition.x - previousMousePosition.x);
            camera.target.y -= (mousePosition.y - previousMousePosition.y);
        }
        if (IsKeyDown(KEY_N) && !nKeyPressed) {
            if (treeStateNext(t) != 1) {
                generateHuffmanCodes(t->root, "", 0);
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