#include <stdbool.h>

typedef struct node
{
    bool leaf;
    char c;
    unsigned freq;
    struct node *left;
    struct node *right;
} node;

typedef struct tree
{
    node *root;
    node **nodes;
    int size;
} tree;

tree *treeInit(const char *);
void freeTree(tree *);
int treeStateNext(tree *t);
unsigned int codeGen(const char);
void generateHuffmanCodes(node *, char[][256], char *, int);
