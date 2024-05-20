#include "set.h"

typedef struct BST_node{
    int key;
    Department *dep;
    struct BST_node *left;
    struct BST_node *right;
}BST_node;

typedef struct BST{
    BST_node *root;
}BST;

BST *create_bst();
Status insert_bst(BST *tree, int key, Department *dep);
Status search_bst(BST *tree, int key, Department **dep);
void free_bst(BST *tree);

BST_node *create_bst_node(int key, Department *dep);
void search_bst_node(BST *tree, int key, BST_node **node);
void free_bst_node(BST_node *node);