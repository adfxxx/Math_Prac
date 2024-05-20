#include "heap.h"

typedef struct Treap_node{
    Data *data;
    int prior;
    struct Treap_node *left;
    struct Treap_node *right;
}Treap_node;

typedef struct Treap_heap{
    Treap_node *root;
    int size;
}Treap_heap;

Treap_heap *create_treap();
Status insert_treap(Treap_heap **heap, Data *data);
Status delete_max_treap(Treap_heap **heap, Data **data);
Status find_max_treap(Treap_heap **heap, Data **data);
Status merge_treap(Treap_heap **result, Treap_heap **heap_1, Treap_heap **heap_2);
Status meld_treap(Treap_heap **result, Treap_heap **heap_1, Treap_heap **heap_2);
void free_treap(Treap_heap **heap);

Treap_node *create_treap_node(Data *data);
Status split_treap(Treap_node *node, Data *data, Treap_node **first, Treap_node **second);
Status copy_treap(Treap_heap **copy, Treap_heap *to_copy);
Status copy_treap_node (Treap_node **copy, Treap_node *to_copy);
Treap_node *merge_treap_node(Treap_node *first, Treap_node *second);
void free_treap_node(Treap_node *node);