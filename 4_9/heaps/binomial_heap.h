#include "heap.h"

typedef struct Binomial_node{
    Data *data;
    int rank;
    struct Binomial_node *child;
    struct Binomial_node *brother;
}Binomial_node;

typedef struct Binomial_heap{
    Binomial_node *root;
    Binomial_node *max;
}Binomial_heap;

Binomial_heap *create_binomial_heap();
Binomial_node *create_binomial_node(Data *data);
Status insert_binomial_heap(Binomial_heap **heap, Data *data);
Status delete_max_binomial_heap(Binomial_heap **heap, Data **data);
Status find_max_binomial_heap(Binomial_heap **heap, Data **data);
Status merge_binomial_heap(Binomial_heap **result, Binomial_heap **heap_1, Binomial_heap **heap_2);
Status meld_binomial_heap(Binomial_heap **result, Binomial_heap **heap_1, Binomial_heap **heap_2);
void free_binomial_heap(Binomial_heap **heap);

Binomial_node *get_max_binomial_heap(Binomial_heap **heap);
Binomial_heap *copy_binomial_heap(Binomial_heap *heap);
Status copy_binomial_node(Binomial_node **end, Binomial_node *start);
void free_binomial_node(Binomial_node *node);