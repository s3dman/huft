#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    bool leaf;
    char c;
    unsigned freq;
    struct node* left;
    struct node* right;
} node;

node* createNode(char c, unsigned freq) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->leaf = true;
    newNode->c = c;
    newNode->freq = freq;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

node* mergeNodes(node* left, node* right) {
    node* newNode = createNode('\0', left->freq + right->freq);
    newNode->leaf = false;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

void displayTree(node* root, int level) {
    if (root != NULL) {
        if (root->leaf) {
            printf("Character: %c, Frequency: %u, Level: %d\n", root->c, root->freq, level);
        } else {
            printf("Frequency: %u, Level: %d\n", root->freq, level);
        }
        displayTree(root->left, level + 1);
        displayTree(root->right, level + 1);
    }
}

void freeTree(node* root) {
    if(root==NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// int main() {
struct node *createtree() {
    char chars[] = {'A', 'B', 'C','D', 'E'};
    unsigned freqs[] = { 5, 8, 2, 4, 3};
    int size = sizeof(chars) / sizeof(chars[0]);

    node* nodes[size];
    for (int i = 0; i < size; i++) {
        nodes[i] = createNode(chars[i], freqs[i]);
    }

    int nodeCount = size;
    node* huffmanTree = NULL;

    while (nodeCount > 1) {
        char input[10];
        scanf("%s", input);
        if (strcmp(input, "p") == 0) {
            for (int i = 0; i < nodeCount - 1; i++) {
                for (int j = i + 1; j < nodeCount; j++) {
                    if (nodes[i]->freq < nodes[j]->freq) {
                        node* temp = nodes[i];
                        nodes[i] = nodes[j];
                        nodes[j] = temp;
                    }
                }
            }

            printf("%d %d\n", nodes[nodeCount - 1]->freq, nodes[nodeCount - 2]->freq);
            node* newNode = mergeNodes(nodes[nodeCount - 1], nodes[nodeCount - 2]);
            nodes[nodeCount - 2] = newNode;
            nodeCount--;

            for (int k = 0; k < nodeCount; k++) {
                printf("%d ", nodes[k]->freq);
            }
            printf("\n");

            huffmanTree = newNode;
            displayTree(huffmanTree, 0);

        } else if (strcmp(input, "q") == 0) {
            break;
        } else {
            printf("Invalid input.\n");
        }
    }

    return huffmanTree;
    if (huffmanTree) freeTree(huffmanTree);
    return 0;

    // return NULL;
}