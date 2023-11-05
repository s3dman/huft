#include "raylib.h"
// #include "binarytree.c" // Include the file that constructs the tree
#include "huffman.c"

#define screenWidth 800
#define screenHeight 600
#define radius 25
#define horizontalSpacing 80
#define verticalSpacing 50

void drawNode(struct node *root, int x, int y) {
    if (root != NULL) {
        // Draw the circle representing the node
        DrawCircle(x, y, radius, RED);
        // DrawText(TextFormat("%d", root->data), x - 10, y - 10, 20, WHITE);
        if(root->leaf==true)
            DrawText(TextFormat("%c", root->c), x - 10, y - 10, 20, WHITE);
        else
            DrawText(TextFormat("%d", root->freq), x - 10, y - 10, 20, WHITE);

        if (root->left != NULL) {
            DrawLine(x - horizontalSpacing, y + verticalSpacing, x, y,GREEN);
            drawNode(root->left, x - horizontalSpacing, y + verticalSpacing);
        }

        if (root->right != NULL) {
            DrawLine(x + horizontalSpacing, y + verticalSpacing, x, y,GREEN);
            drawNode(root->right, x + horizontalSpacing, y + verticalSpacing);
        }
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Binary Tree Visualization");
    SetTargetFPS(60);

    struct node *root = createtree(); // Get the root node from the constructed tree

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        drawNode(root, screenWidth / 2, 50); // Draw the tree

        EndDrawing();
    }

    CloseWindow();

    return 0;
}