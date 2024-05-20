#include "heap.h"

typedef struct Leftist_node{
    Data *data;
    int dist;
    struct Leftist_node *left;
    struct Leftist_node *right;
}Leftist_node;

typedef struct Leftist_heap{
    Leftist_node *root;
    int size;
}Leftist_heap;

Leftist_heap *create_leftist_heap();
Status insert_leftist_heap(Leftist_heap **heap, Data *data);
Status delete_max_leftist_heap(Leftist_heap **heap, Data **data);
Status find_max_leftist_heap(Leftist_heap **heap, Data **data);
Status merge_leftist_heap(Leftist_heap **result, Leftist_heap **heap_1, Leftist_heap **heap_2);
Status meld_leftist_heap(Leftist_heap **result, Leftist_heap **first, Leftist_heap **second);
void free_leftist_heap(Leftist_heap **heap);

Leftist_node *create_leftist_node(Data *data);
Leftist_node *merge_leftist_node(Leftist_node *first, Leftist_node *second);
Status copy_leftist_heap(Leftist_heap **copy, Leftist_heap *to_copy);
Status copy_leftist_node(Leftist_node **copy, Leftist_node *to_copy);
void free_leftist_node(Leftist_node *node);