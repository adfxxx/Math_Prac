#include "set.h"
#define SIZE 10

typedef struct Trie_node{
    struct Trie_node *children[SIZE];
    Department *dep;
}Trie_node;

typedef struct Trie_set{
    Trie_node *root;
}Trie_set;

Trie_set *create_trie();
Status insert_trie(Trie_set *tree, int key, Department *dep);
Status search_trie(Trie_set *tree, int key, Department **res);
void free_trie(Trie_set *tree);

Trie_node *create_trie_node();
void free_trie_node(Trie_node *node);
int get_digits_amount(int num);
