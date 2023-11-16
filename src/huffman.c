#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huffman.h"

node *createNode(char c, unsigned freq)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->leaf = true;
    newNode->c = c;
    newNode->freq = freq;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

node *mergeNodes(node *left, node *right)
{
    node *newNode = createNode('\0', left->freq + right->freq);
    newNode->leaf = false;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

void displayTree(node *root, int level)
{   // DEBUG
    if (root != NULL)
    {
        if (root->leaf)
        {
            printf("Character: %c, Frequency: %u, Level: %d\n", root->c, root->freq, level);
        }
        else
        {
            printf("Frequency: %u, Level: %d\n", root->freq, level);
        }
        displayTree(root->left, level + 1);
        displayTree(root->right, level + 1);
    }
}

void generateHuffmanCodes(node *root, char huffCodeArr[][256], char *code, int depth)
{
    static char codeBuffer[256];
    if (root == NULL)
        return;
    if (root->leaf)
    {
        codeBuffer[depth] = '\0';
        strcpy(huffCodeArr[root->c], codeBuffer);
    }
    codeBuffer[depth] = '0';
    generateHuffmanCodes(root->left, huffCodeArr, codeBuffer, depth + 1);
    codeBuffer[depth] = '1';
    generateHuffmanCodes(root->right, huffCodeArr, codeBuffer, depth + 1);
}

void freeNodes(node *root)
{
    if (root == NULL)
        return;
    freeNodes(root->left);
    freeNodes(root->right);
    free(root);
}

void freeTree(tree *tree)
{
    freeNodes(tree->root);
    free(tree->nodes);
    free(tree);
}

tree *treeInit(const char *input)
{
    size_t length = strlen(input);
    char *chars = (char *)malloc(length * sizeof(char));
    int *freqs = (int *)malloc(length * sizeof(int));

    if (chars == NULL || freqs == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(0);
    }

    for (int i = 0; i < length; i++)
    {
        chars[i] = '\0';
        freqs[i] = 0;
    }

    int end = 0;

    for (int i = 0; i < length; i++)
    {
        char currentChar = input[i];
        bool found = false;

        for (int j = 0; j < end; j++)
        {
            if (chars[j] == currentChar)
            {
                freqs[j]++;
                found = true;
            }
        }

        if (!found)
        {
            chars[end] = currentChar;
            freqs[end] = 1;
            end++;
        }
    }

    chars = (char *)realloc(chars, end * sizeof(char));
    freqs = (int *)realloc(freqs, end * sizeof(int));

    node **nodes = (node **)malloc(end * sizeof(node *));
    for (int i = 0; i < end; i++)
    {
        nodes[i] = createNode(chars[i], freqs[i]);
    }

    tree *t = (tree *)malloc(sizeof(tree));
    t->root = NULL;
    t->nodes = nodes;
    t->size = end;

    free(chars);
    free(freqs);

    return t;
}

int treeStateNext(tree *t)
{
    if (t->size <= 1)
    {
        return 1;
    }
    for (int i = 0; i < t->size - 1; i++)
    {
        for (int j = i + 1; j < t->size; j++)
        {
            if (t->nodes[i]->freq < t->nodes[j]->freq)
            {
                node *temp = t->nodes[i];
                t->nodes[i] = t->nodes[j];
                t->nodes[j] = temp;
            }
        }
    }

    node *newNode = mergeNodes(t->nodes[t->size - 1], t->nodes[t->size - 2]);
    t->nodes[t->size - 2] = newNode;
    t->size--;

    t->root = newNode;
    return 0;
}