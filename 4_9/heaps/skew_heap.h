#include "heap.h"

typedef struct Skew_node{
    Data *data;
    struct Skew_node *left;
    struct Skew_node *right;
}Skew_node;

typedef struct Skew_heap{
    struct Skew_node *root;
    int size;
}Skew_heap;

Skew_heap *create_skew_heap();
Status insert_skew_heap(Skew_heap **heap, Data *data);
Status delete_max_skew_heap(Skew_heap **heap, Data **result);
Status find_max_skew_heap(Skew_heap **heap, Data **data);
Status merge_skew_heap(Skew_heap **result, Skew_heap **heap_1, Skew_heap **heap_2);
Status meld_skew_heap(Skew_heap **result, Skew_heap **first, Skew_heap **second);
void free_skew_heap(Skew_heap **heap);

Skew_node *create_skew_node(Data *data);
Skew_node *merge_skew_node(Skew_node *first, Skew_node *second);
Status copy_skew_heap(Skew_heap **copy, Skew_heap *to_copy);
Status copy_skew_node(Skew_node **copy, Skew_node *to_copy);
void free_skew_node(Skew_node *node);