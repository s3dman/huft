#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

#define screenWidth 1280
#define screenHeight 720
#define radius 20

#define verticalSpacing 100

void nodeOverlay(int x,int y,char c, unsigned int f,Vector2 rpos,Font ttf,char huffCodeArr[][256]) {
    if(CheckCollisionPointCircle(rpos,(Vector2) {
    x,y
},radius)) {
        const char* overlayText = TextFormat("freq: %d\nchar: '%c'\ncode: %s",f,c,huffCodeArr[c]);
        if(c == '\0') overlayText = TextFormat("PARENT\nfreq: %d",f);
        Vector2 overlayDim =  MeasureTextEx( ttf,overlayText, 18,0);
        DrawRectangle(rpos.x-50,rpos.y,100,100, GetColor(0xffffff33) );
        DrawTextEx(ttf,overlayText, (Vector2) {
            rpos.x-overlayDim.x/2, rpos.y+50-overlayDim.y/2
        }, 18,0, WHITE);
    }

}

void drawNode(node* root, int x, int y, int level,Vector2 rpos,Font ttf,char huffCodeArr[][256]) {
    if (root != NULL) {
        int dynamicSpacing = screenWidth / (1 << (level+2));

        const char* tf = NULL;
        Vector2 tfw;



        if(root->leaf) {
            tf = TextFormat("%c",root->c);
            tfw =  MeasureTextEx( ttf,tf, 32,0);
            DrawCircle(x, y, radius, GREEN);
            DrawTextEx(ttf,tf, (Vector2) {
                x-tfw.x/2, y-16
            }, 32,0, BLACK);

        }
        else {
            if (root->left != NULL) {
                tf = TextFormat("%d",root->freq);
                tfw=  MeasureTextEx( ttf,tf, 32,0);
                DrawLine(x, y, x-dynamicSpacing, y+verticalSpacing, YELLOW);
                DrawCircle(x, y, radius, PINK);
                DrawTextEx(ttf,tf, (Vector2) {
                    x-tfw.x/2, y-16
                }, 32,0, BLACK);
                drawNode(root->left, x - dynamicSpacing, y + verticalSpacing, level + 1,rpos,ttf,huffCodeArr);
            }

            if (root->right != NULL) {
                tf = TextFormat("%d",root->freq);
                tfw=  MeasureTextEx( ttf,tf, 32,0);
                DrawLine(x, y, x+dynamicSpacing, y+verticalSpacing, YELLOW);
                DrawCircle(x, y, radius, PINK);
                DrawTextEx(ttf,tf, (Vector2) {
                    x-tfw.x/2, y-16
                }, 32,0, BLACK);
                drawNode(root->right, x + dynamicSpacing, y + verticalSpacing, level + 1,rpos,ttf,huffCodeArr);
            }

        }

        nodeOverlay(x,y,root->c,root->freq,rpos,ttf,huffCodeArr);
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

    char ar[] = "my name is ademulla";
    tree *t = treeInit(ar);
    char huffCodeArr[256][256];
    memset(huffCodeArr, '\0', sizeof(huffCodeArr)); // init with \0

    bool nKeyPressed = false;
    Font jbmTtf = LoadFontEx("../assets/JetBrainsMono-Regular.ttf", 32, 0, 0);

    while (!WindowShouldClose()) {
        previousMousePosition = mousePosition;
        mousePosition = GetMousePosition();
        Vector2 worldMousePos = GetScreenToWorld2D(mousePosition, camera);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        drawNode(t->root, screenWidth / 2, 50, 0,worldMousePos, jbmTtf,huffCodeArr);
        for(int i=0; i<t->size; i++) {
            const char* a = TextFormat("{f:%d-c:%c}", t->nodes[i]->freq,t->nodes[i]->c);
            if(!t->nodes[i]->leaf)
                a = TextFormat("{f:%d}", t->nodes[i]->freq);
            DrawTextEx(jbmTtf,a, (Vector2) {
                0,32*i
            },32.0f,0, WHITE);
        }

        EndMode2D();
        EndDrawing();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            camera.target.x -= (mousePosition.x - previousMousePosition.x);
            camera.target.y -= (mousePosition.y - previousMousePosition.y);
        }
        if (IsKeyDown(KEY_N) && !nKeyPressed) {
            if (treeStateNext(t) != 1) {
                generateHuffmanCodes(t->root,huffCodeArr, "", 0);
            }
            nKeyPressed = true;
        }
        if (!IsKeyDown(KEY_N)) nKeyPressed = false;
    }

    freeTree(t);
    CloseWindow();
    return 0;
}