#ifndef SET_H
#define SET_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../structs/department.h"

typedef struct Set{
    void *dep;

    void* (*create)();
    void (*free)(void *dep);
    int (*insert)(void *dep, int key, Department *dp);
    int (*find)(void *dep, int key, Department **dp);
}Set;

typedef enum Set_type{
    Hash,
    Array,
    Bst,
    Trie,
    ERROR_SET
}Set_type;

int create_set(Set *set, Set_type type);
Status insert_set(Set *set, Data *data);
Status free_set(Set *set);

#endif