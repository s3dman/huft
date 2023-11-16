#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

#define screenWidth 1680
#define screenHeight 1000
#define radius 20

#define verticalSpacing 100
#define MAX_INPUT_CHARS 128

v

    void
    drawNode(node *root, int x, int y, int level, Vector2 rpos, Font ttf, char huffCodeArr[][256])
{
    if (root != NULL)
    {
        int dynamicSpacing = screenWidth / (1 << (level + 2));

        const char *tf = NULL;
        Vector2 tfw;

        if (root->leaf)
        {
            tf = TextFormat("%c", root->c);
            tfw = MeasureTextEx(ttf, tf, 32, 0);
            DrawCircle(x, y, radius, GREEN);
            DrawTextEx(ttf, tf, (Vector2){x - tfw.x / 2, y - 16}, 32, 0, BLACK);
        }
        else
        {
            if (root->left != NULL)
            {
                tf = TextFormat("%d", root->freq);
                tfw = MeasureTextEx(ttf, tf, 32, 0);
                DrawLine(x, y, x - dynamicSpacing, y + verticalSpacing, YELLOW);
                DrawCircle(x, y, radius, PINK);
                DrawTextEx(ttf, tf, (Vector2){x - tfw.x / 2, y - 16}, 32, 0, BLACK);
                drawNode(root->left, x - dynamicSpacing, y + verticalSpacing, level + 1, rpos, ttf, huffCodeArr);
            }

            if (root->right != NULL)
            {
                tf = TextFormat("%d", root->freq);
                tfw = MeasureTextEx(ttf, tf, 32, 0);
                DrawLine(x, y, x + dynamicSpacing, y + verticalSpacing, YELLOW);
                DrawCircle(x, y, radius, PINK);
                DrawTextEx(ttf, tf, (Vector2){x - tfw.x / 2, y - 16}, 32, 0, BLACK);
                drawNode(root->right, x + dynamicSpacing, y + verticalSpacing, level + 1, rpos, ttf, huffCodeArr);
            }
        }

        nodeOverlay(x, y, root->c, root->freq, rpos, ttf, huffCodeArr);
    }
}

int main()
{
    InitWindow(screenWidth, screenHeight, "huft");

    int state = 0;

    Camera2D camera = {0};
    camera.target = (Vector2){
        0.0f, 0.0f};

    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Vector2 mousePosition = {0.0f, 0.0f};
    Vector2 previousMousePosition = {0.0f, 0.0f};

    Font jbmTtf = LoadFontEx("../assets/JetBrainsMono-Regular.ttf", 32, 0, 0);

    SetTargetFPS(60);

    char ar[1024];
    tree *t;
    int treeComplete = 0;

    char huffCodeArr[256][256];
    memset(huffCodeArr, '\0', sizeof(huffCodeArr)); // init with \0
    char encodedString[1024];
    Vector2 spacing;

    bool nKeyPressed = false;

    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;

    while (!WindowShouldClose())
    {

        if (state == 0)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }
                key = GetCharPressed(); // Check next character in the queue
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                name[letterCount] = '\0';
            }
        }

        previousMousePosition = mousePosition;
        mousePosition = GetMousePosition();
        Vector2 worldMousePos = GetScreenToWorld2D(mousePosition, camera);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        if (state == 1 || state == 2)
        {
            drawNode(t->root, screenWidth / 2, 50, 0, worldMousePos, jbmTtf, huffCodeArr);
            //
            //     for(int i=0; i<t->size; i++) { // priority queue display
            //         const char* a = TextFormat("{f:%d-c:%c}", t->nodes[i]->freq,t->nodes[i]->c);
            //         if(!t->nodes[i]->leaf)
            //             a = TextFormat("{f:%d}", t->nodes[i]->freq);
            //         DrawTextEx(jbmTtf,a, (Vector2) {
            //             0,32*i
            //         },32.0f,0, WHITE);
            //     }
        }

        if (treeComplete == 1)
        {
            strcpy(encodedString, "ENCODED STRING: ");
            for (int i = 0; i < strlen(ar); i++)
                strcat(encodedString, huffCodeArr[ar[i]]);
            if (strlen(encodedString) <= 16)
                strcpy(encodedString, "INVALID STRING");
            spacing = MeasureTextEx(jbmTtf, encodedString, 16, 0);
            printf("%s\n", encodedString);
            treeComplete++;
        }

        EndMode2D();

        char displayString[1024] = "INPUT STRING: ";
        strcat(displayString, name);
        Vector2 inputSpacing = MeasureTextEx(jbmTtf, displayString, 16, 0);
        DrawTextEx(jbmTtf, displayString, (Vector2){(screenWidth - inputSpacing.x) / 2, screenHeight - 4 * inputSpacing.y}, 16, 0, WHITE); // input string

        DrawTextEx(jbmTtf, encodedString, (Vector2){(screenWidth - spacing.x) / 2, screenHeight - 2 * spacing.y}, 16, 0, WHITE); // output string

        EndDrawing();

        if (state == 0)
        {
            if (IsKeyDown(KEY_ENTER))
            {
                strcpy(ar, name);
                t = treeInit(ar);
                treeComplete = 0;
                strcpy(encodedString, "...");
                spacing = MeasureTextEx(jbmTtf, encodedString, 16, 0);
                state++;
                if (treeStateNext(t) == 1)
                {
                    generateHuffmanCodes(t->root, huffCodeArr, "", 0);
                    treeComplete++;
                    state++;
                }
            }
        }
        if (state == 1)
        {
            if (IsKeyDown(KEY_ENTER) && !nKeyPressed)
            {
                if (treeStateNext(t) == 1)
                {
                    strcpy(encodedString, "");
                    generateHuffmanCodes(t->root, huffCodeArr, "", 0);
                    treeComplete++;
                    state++;
                }
                nKeyPressed = true;
            }
            if (!IsKeyDown(KEY_ENTER))
                nKeyPressed = false;
        }
        if (state == 2)
        {
            if (IsKeyDown(KEY_BACKSPACE))
            {
                freeTree(t);
                t = NULL;
                memset(huffCodeArr, '\0', sizeof(huffCodeArr));
                strcpy(name, "\0");
                strcpy(encodedString, "");
                letterCount = 0;
                state = 0;
            }
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            camera.target.x -= (mousePosition.x - previousMousePosition.x);
            camera.target.y -= (mousePosition.y - previousMousePosition.y);
        }
    }
    if (t != NULL)
        freeTree(t);
    CloseWindow();
    return 0;
}