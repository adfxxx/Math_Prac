#include "heap.h"

typedef struct{
    Data **info;
    int capacity;
    int size;
}Binary_heap;

Binary_heap *create_binary_heap();
Status insert_binary_heap(Binary_heap **heap, Data *data);
Status delete_max_binary_heap(Binary_heap **heap, Data **data);
Status find_max_binary_heap(Binary_heap **heap, Data **data);
Status merge_binary_heap(Binary_heap **result, Binary_heap **heap_1, Binary_heap **heap_2);
Status meld_binary_heap(Binary_heap **result, Binary_heap **heap_1, Binary_heap **heap_2);
void free_binary_heap(Binary_heap **heap);

Status heapify_up_binary_heap(Binary_heap **heap);
Status heapify_down_binary_heap(Binary_heap **heap, int index);