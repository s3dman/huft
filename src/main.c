#include "raylib.h"
// #include "binarytree.c" // Include the file that constructs the tree
#include "huffman.c"

#define screenWidth 800
#define screenHeight 600
#define radius 25

#define verticalSpacing 100
#define textsize 20

int getMaxDepth(struct node *root) {
    if (root == NULL) {
        return 0;
    } else {
        int leftDepth = getMaxDepth(root->left);
        int rightDepth = getMaxDepth(root->right);

        if (leftDepth > rightDepth) {
            return leftDepth + 1;
        } else {
            return rightDepth + 1;
        }
    }
}

void drawNode(struct node *root, int x, int y, int level, int depth) {
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
            drawNode(root->left, x - dynamicSpacing, y + verticalSpacing, level + 1, depth);
        }

        if (root->right != NULL) {
            DrawLine(x + dynamicSpacing, y + verticalSpacing, x, y, GREEN);
            drawNode(root->right, x + dynamicSpacing, y + verticalSpacing, level + 1, depth);
        }
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Binary Tree Visualization");

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

    struct node *root = createTree();

    int depth = getMaxDepth(root);
    while (!WindowShouldClose()) {
        previousMousePosition = mousePosition;
        mousePosition = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        drawNode(root, screenWidth / 2, 50, 0, depth);
        EndMode2D();
        EndDrawing();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            camera.target.x += (mousePosition.x - previousMousePosition.x);
            camera.target.y += (mousePosition.y - previousMousePosition.y);
        }
    }

    CloseWindow();

    return 0;
}