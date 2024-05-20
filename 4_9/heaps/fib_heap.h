#include <math.h>
#include "heap.h"

typedef struct Fib_node{
    Data *data;
    int rank;
    struct Fib_node *child;
    struct Fib_node *left;
    struct Fib_node *right;
}Fib_node;

typedef struct{
    Fib_node *root;
    int count;
}Fib_heap;

Fib_heap *create_fib_heap();
Status insert_fib_heap(Fib_heap **heap, Data *data);
Status delete_max_fib_heap(Fib_heap **heap, Data **result);
Status find_max_fib_heap(Fib_heap **heap, Data **data);
Status merge_fib_heap(Fib_heap **result, Fib_heap **heap_1, Fib_heap **heap_2);
Status meld_fib_heap(Fib_heap **result, Fib_heap **heap_1, Fib_heap **heap_2);
void free_fib_heap(Fib_heap **heap);

Fib_node *create_fib_node(Data *data);
Status merge_fib_node(Fib_heap **heap, Fib_node *first, Fib_node *second, Fib_node **res);
Status consolidate_fib_heap(Fib_heap **heap);
Status copy_fib_heap(Fib_heap **copy, Fib_heap *to_copy);
Status copy_fib_node(Fib_node **copy, Fib_node *to_copy);
void free_fib_node(Fib_node *node);