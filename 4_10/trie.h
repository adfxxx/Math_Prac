#include <stdio.h>
#include <stdlib.h>
#include "status.h"

#define SIZE 63

typedef struct Trie_node{
    struct Trie_node *children[SIZE];
    unsigned int value;
    int end;
}Trie_node;

typedef struct Trie{
    Trie_node *root;
}Trie;

Trie *create_trie();
Status insert_trie(Trie *tree, char *key, unsigned int value);
Status search_trie(Trie *tree, char *key, Trie_node **res);
void free_trie(Trie *tree);

Trie_node *create_trie_node();
void free_trie_node(Trie_node *node);
