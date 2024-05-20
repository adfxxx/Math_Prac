#include "set.h"

typedef struct D_array_elem{
    int key;
    Department *dep;
}D_array_elem;

typedef struct D_array{
    int capacity;
    int size;
    D_array_elem **elem;
}D_array;

D_array *create_array();
Status insert_array(D_array *arr, int key, Department *dep);
Status search_array(D_array *arr, int key, Department **res);
void free_array(D_array *arr);

D_array_elem **create_array_elem(int size);
D_array_elem *create_array_elem_in(int key, Department *dep);
Status change_array_elem_size(D_array_elem ***arr_elem, int size);
void free_array_elem(D_array_elem **arr, int size);
void free_array_elem_in(D_array_elem *elem);