#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include "../structs/data.h"

typedef enum Heap_type{
    Binary,
    Binomial,
    Fibonacci,
    Leftist,
    Skew,
    Treap,
    ERROR_HEAP
}Heap_type;

typedef struct Heap{
    void *dep;
    Heap_type type;
    int size;

    Status (*create) (void **dep);
    void (*free)(void **dep);
    Status(*find_max)(void **dep, Data **data);
    Status (*delete_max)(void **dep, Data **data);
    Status (*merge)(void **result, void **first, void **second);
    Status (*meld)(void **result, void **first, void **second);
    Status (*insert)(void **dep, Data *data);
}Heap;

void free_heap(Heap *h);
Status create_heap(Heap *h, Heap_type type);

#endif