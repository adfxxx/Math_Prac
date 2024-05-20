#include "binary_heap.h"

Binary_heap *create_binary_heap(){
    Binary_heap *heap = (Binary_heap*)malloc(sizeof(Binary_heap));
    if(!heap){
        return NULL;
    }
    heap->capacity = 32;
    heap->size = 0;
    heap->info = (Data**)calloc(heap->capacity,sizeof(Data*));
    if(!(heap->info)){
        free(heap);
        return NULL;
    }
    return heap;
}

Status insert_binary_heap(Binary_heap **heap, Data *data){
    if(!heap){
        return wrong_argument;
    }
    if((*heap)->size == (*heap)->capacity){
        Data **tmp = (Data**)realloc((*heap)->info, sizeof(Data*)*((*heap)->capacity*2));
        if(!tmp){
            return error_allocation;
        }
        (*heap)->info = tmp;
        (*heap)->capacity *= 2;
    }
    (*heap)->info[(*heap)->size] = data;
    heapify_up_binary_heap(heap);
    (*heap)->size++;
    return success;
}

Status heapify_up_binary_heap(Binary_heap **heap){
    if(!(*heap)){
        return wrong_argument;
    }

    int index = (*heap)->size;
    while(index > 0 && compare_data((*heap)->info[(index-1)/2], (*heap)->info[index]) < 0){
        Data *tmp = (*heap)->info[index];
        (*heap)->info[index] = (*heap)->info[(index-1)/2];
        (*heap)->info[(index-1)/2] = tmp;
        index = (index - 1)/2;
    }
    return success;
}

Status heapify_down_binary_heap(Binary_heap **heap, int index){
    if(!(*heap)){
        return wrong_argument;
    }

    while(2*index + 1 < (*heap)->size){
        int left_index = 2*index + 1;
        int right_index = 2*index + 2;
        int smallest = left_index;   
        if(right_index < (*heap)->size && compare_data((*heap)->info[right_index], (*heap)->info[left_index]) > 0){
            smallest = right_index;
        }
        if(compare_data((*heap)->info[index], (*heap)->info[smallest]) > 0){
            break;
        }
        Data *tmp = (*heap)->info[index];
        (*heap)->info[index] = (*heap)->info[smallest];
        (*heap)->info[smallest] = tmp;
        index = smallest;
    }
    return success;
    
}

Status delete_max_binary_heap(Binary_heap **heap, Data **data){
    if(find_max_binary_heap(heap, data) != success){
        return wrong_argument;
    }
    (*heap)->info[0] = (*heap)->info[(*heap)->size - 1];
    heapify_down_binary_heap(heap, 0);
    (*heap)->size--;

    return success;
}

Status find_max_binary_heap(Binary_heap **heap, Data **data){
    if(!(*heap)){
        return wrong_argument;
    }
    *data = ((*heap)->info)[0];
    return success;
}

Status merge_binary_heap(Binary_heap **result, Binary_heap **heap_1, Binary_heap **heap_2){
    if(*result || !(*heap_1) || !(*heap_2) || *heap_1 == *heap_2){
        free_binary_heap(heap_1);
        free_binary_heap(heap_2);
        return wrong_argument;
    }
    int capacity = ((*heap_1)->capacity > (*heap_2)->capacity) ? (*heap_1)->capacity : (*heap_2)->capacity;

    if((*heap_1)->size + (*heap_2)->size > capacity){
        capacity = ((*heap_1)->size + (*heap_2)->size)*2;
    }

    Binary_heap *tmp = create_binary_heap();
    if(!tmp){
        free_binary_heap(heap_1);
        free_binary_heap(heap_2);
        return error_allocation;
    }

    tmp->size = (*heap_1)->size + (*heap_2)->size;
    tmp->capacity = capacity;
    tmp->info = (Data**)calloc(capacity, sizeof(Data*));
    if(!(tmp->info)){
        free(tmp);
        tmp = NULL;
        free_binary_heap(heap_1);
        free_binary_heap(heap_2);
        return error_allocation;
    }

    for(int i = 0; i < (*heap_1)->size; i++){
        tmp->info[i] = copy_data((*heap_1)->info[i]);
    }
    for (int i = 0; i < (*heap_2)->size; i++){
        tmp->info[i + (*heap_2)->size] = copy_data((*heap_2)->info[i]);
    }
    for(int i = 0; i < tmp->size/2;i++){
        heapify_down_binary_heap(&tmp, tmp->size/2 - i);
    }
    *result = tmp;
    return success;
}

Status meld_binary_heap(Binary_heap **result, Binary_heap **heap_1, Binary_heap **heap_2){
    if(*result || !(*heap_1) || !(*heap_2) || *heap_1 == *heap_2){
        free_binary_heap(heap_1);
        free_binary_heap(heap_2);
        return wrong_argument;
    }
    int capacity = ((*heap_1)->capacity > (*heap_2)->capacity) ? (*heap_1)->capacity : (*heap_2)->capacity;

    if((*heap_1)->size + (*heap_2)->size > capacity){
        capacity = ((*heap_1)->size + (*heap_2)->size)*2;
    }

    Binary_heap *tmp = create_binary_heap();
    if(!tmp){
        free_binary_heap(heap_1);
        free_binary_heap(heap_2);
        return error_allocation;
    }

    tmp->size = (*heap_1)->size + (*heap_2)->size;
    tmp->capacity = capacity;
    tmp->info = (Data**)calloc(capacity, sizeof(Data*));
    if(!(tmp->info)){
        free(tmp);
        tmp = NULL;
        free_binary_heap(heap_1);
        free_binary_heap(heap_2);
        return error_allocation;
    }

    for(int i = 0; i < (*heap_1)->size; i++){
        tmp->info[i] = copy_data((*heap_1)->info[i]);
    }
    for (int i = 0; i < (*heap_2)->size; i++){
        tmp->info[i + (*heap_2)->size] = copy_data((*heap_2)->info[i]);
    }
    for(int i = 0; i < tmp->size/2;i++){
        heapify_down_binary_heap(&tmp, tmp->size/2 - i);
    }

    free_binary_heap(heap_1);
    free_binary_heap(heap_2);

    *result = tmp;
    return success;
}

void free_binary_heap(Binary_heap **heap){
    if(!(*heap)){
        return;
    }
    if((*heap)->info){
        for(int i = 0; i < (*heap)->size; i++){
            free_data((*heap)->info[i]);
        }
        free((*heap)->info);
    }
    free(*heap);
    *heap = NULL;
}