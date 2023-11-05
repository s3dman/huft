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

typedef struct tree {
    node* root;
    node** nodes;
    int size;
} tree;

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
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

tree* treeInit(const char* input) {
    size_t length = strlen(input);
    char* chars = (char*)malloc(length * sizeof(char));
    int* freqs = (int*)malloc(length * sizeof(int));

    if (chars == NULL || freqs == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(0);
    }

    for (int i = 0; i < length; i++) {
        chars[i] = '\0';
        freqs[i] = 0;
    }

    int end = 0;

    for (int i = 0; i < length; i++) {
        char currentChar = input[i];
        bool found = false;

        for (int j = 0; j < end; j++) {
            if (chars[j] == currentChar) {
                freqs[j]++;
                found = true;
            }
        }

        if (!found) {
            chars[end] = currentChar;
            freqs[end] = 1;
            end++;
        }
    }

    chars = (char*)realloc(chars, end * sizeof(char));
    freqs = (int*)realloc(freqs, end * sizeof(int));

    node** nodes = (node**)malloc(end * sizeof(node*));
    for (int i = 0; i < end; i++) {
        nodes[i] = createNode(chars[i], freqs[i]);
    }

    tree* t = (tree*)malloc(sizeof(tree));
    t->root = NULL;
    t->nodes = nodes;
    t->size = end;

    free(chars);
    free(freqs);

    return t;
}

int treeStateNext(tree* t) {
    if(t->size <= 1) {
        displayTree(t->root, 0);
        return 1;
    }
    for (int i = 0; i < t->size - 1; i++) {
        for (int j = i + 1; j < t->size; j++) {
            if (t->nodes[i]->freq < t->nodes[j]->freq) {
                node* temp = t->nodes[i];
                t->nodes[i] = t->nodes[j];
                t->nodes[j] = temp;
            }
        }
    }

    for (int k = 0; k < t->size; k++) {
        printf("%d ", t->nodes[k]->freq);
    }
    printf("\n");

    node* newNode = mergeNodes(t->nodes[t->size - 1], t->nodes[t->size - 2]);
    t->nodes[t->size - 2] = newNode;
    t->size--;

    for (int k = 0; k < t->size; k++) {
        printf("%d ", t->nodes[k]->freq);
    }
    printf("\n");

    t->root = newNode;
    displayTree(t->root, 0);
    return 0;
}

int main() {
    char ar[] = "abbcccddddeeeee";
    tree* t = treeInit(ar);
    int a;
    while(1) {
        scanf("%d",&a);
        if(a==0) {
            if(treeStateNext(t)) break;
        }
        else break;
    }

    freeTree(t->root);
    free(t->nodes);
    free(t);
    return 0;
}