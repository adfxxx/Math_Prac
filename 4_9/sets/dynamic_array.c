#include "dynamic_array.h"

D_array *create_array(){
    D_array *arr = (D_array*)malloc(sizeof(D_array));
    if(!arr){
        return NULL;
    }

    arr->size = 0;
    arr->capacity = 10;
    arr->elem = create_array_elem(arr->capacity);
    if(!arr->elem){
        free(arr);
        return NULL;
    }
    return arr;
}

D_array_elem **create_array_elem(int size){
    D_array_elem **elem = (D_array_elem**)calloc(size, sizeof(D_array_elem*));
    if(!elem){
        return NULL;
    }
    return elem;
}

D_array_elem *create_array_elem_in(int key, Department *dep){
    D_array_elem *elem = (D_array_elem*)malloc(sizeof(D_array_elem));
    if(!elem){
        return NULL;
    }
    elem->key = key;
    elem->dep = dep;
    return elem;
}

Status change_array_elem_size(D_array_elem ***arr_elem, int size){
    if(!*arr_elem){
        return wrong_argument;
    }

    D_array_elem **tmp =  (D_array_elem**)realloc(*arr_elem, sizeof(D_array_elem*)*size);
    if(!tmp){
        return error_allocation;
    }
    *arr_elem = tmp;
    return success;
}

Status insert_array(D_array *arr, int key, Department *dep){
    D_array_elem *new_elem = create_array_elem_in(key, dep);
    if(!new_elem){
        return error_allocation;
    }

    Status status;
    if(arr->size == arr->capacity){
        status = change_array_elem_size(&arr->elem, arr->capacity*2);
        if(status != success){
            return status;
        }
        arr->capacity *= 2;
    }

    int index = -1;
    for(int i = 0; i < arr->size; i++){
        if(arr->elem[i]->key > key){
            index = i;
            break;
        }
    }

    if(index < 0){
        index = arr->size;
    }
    else{
        int tmp = arr->size + 1;
        while(tmp != index){
            arr->elem[tmp] = arr->elem[tmp-1];
            tmp--;
        }
    }
    arr->elem[index] = new_elem;
    arr->size++;
    return success;
}

Status search_array(D_array *arr, int key, Department **res){
    if(!arr || !arr->elem){
        return wrong_argument;
    }
    int left = 0;
    int right = arr->size - 1;
    int middle = 0;
    while(left <= right){
        middle = left + (right - left)/2;
        if(arr->elem[middle]->key == key){
            *res = arr->elem[middle]->dep;
            return success;
        }
        else if(arr->elem[middle]->key > key){
            right = middle - 1;
        }
        else{
            left = middle + 1;
        }
    }
    
    if(arr->elem[1]->key == key){
        *res = arr->elem[1]->dep;
        return success;
    }

    *res = NULL;
    return data_not_found;
}

void free_array(D_array *arr){
    free_array_elem(arr->elem, arr->size);
    free(arr);
    arr = NULL;
}

void free_array_elem(D_array_elem **arr, int size){
    for(int i = 0; i < size; i++){
        free_array_elem_in(arr[i]);
    }
    free(arr);
    arr = NULL;
}

void free_array_elem_in(D_array_elem *elem){
    if(!elem){
        return;
    }
    free_dep(elem->dep);
    free(elem);
    elem = NULL;
}