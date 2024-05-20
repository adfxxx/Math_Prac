#include "skew_heap.h"

Skew_heap *create_skew_heap(){
    Skew_heap *heap = (Skew_heap*)malloc(sizeof(Skew_heap));
    if(!heap){
        return NULL;
    }
    heap->size = 0;
    heap->root = NULL;
    return heap;
}

Skew_node *create_skew_node(Data *data){
    Skew_node *node = (Skew_node*)malloc(sizeof(Skew_node));
    if(!node){
        free_data(data);
        return NULL;
    }
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

Status insert_skew_heap(Skew_heap **heap, Data *data){
    if(!heap){
        return wrong_argument;
    }
    Skew_node *node = create_skew_node(data);
    if(!node){
        free_skew_heap(heap);
        free_data(data);
        return error_allocation;
    }

    Skew_heap *tmp = create_skew_heap();
    if(!tmp){
        free_skew_heap(heap);
        free_data(data);
        return error_allocation;
    }
    tmp->root = node;
    tmp->size = 1;

    Skew_heap *new_heap = NULL;
    Status status = meld_skew_heap(&new_heap, heap, &tmp);
    if(status != success){
        free_skew_heap(heap);
        free_skew_heap(&tmp);
        free_skew_node(node);
        free_data(data);
        return error_allocation;
    }
    *heap = new_heap;
    return success;
}

Status delete_max_skew_heap(Skew_heap **heap, Data **result){
    if(!heap){
        return wrong_argument;
    }
    if(!(*heap)->root){
        return success;
    }

    *result = (*heap)->root->data;
    Skew_node *root = (*heap)->root;
    (*heap)->root = merge_skew_node((*heap)->root->right, (*heap)->root->left);

    free(root);
    (*heap)->size--;
    return success;
}

Status find_max_skew_heap(Skew_heap **heap, Data **data){
    if(!heap){
        return wrong_argument;
    }
    if(!(*heap)->root){
        return success;
    }
    *data = (*heap)->root->data;
    return success;
}

Skew_node *merge_skew_node(Skew_node *first, Skew_node *second){
    if(!first || !second){
        return NULL;
    }
    if(!first){
        return second;
    }
    if(!second){
        return first;
    }

    if(compare_data(first->data, second->data) < 0){
        Skew_node *tmp = first;
        first = second;
        second = tmp;
    }

    first->left = merge_skew_node(second, first->left);
    Skew_node *tmp = first->left;
    first->left = first->right;
    first->right = tmp;
    return first;
}

Status copy_skew_heap(Skew_heap **copy, Skew_heap *to_copy){
    if(*copy || !to_copy){
        return wrong_argument;
    }

    Status status = copy_skew_node(&(*copy)->root, to_copy->root);
    if(status != success){
        free_skew_heap(copy);
        free_skew_heap(&to_copy);
        return status;
    }

    (*copy)->size = to_copy->size;
    return success;
}

Status copy_skew_node(Skew_node **copy, Skew_node *to_copy){
    if(*copy){
        free_skew_node(to_copy);
        return wrong_argument;
    }
    if(!to_copy){
        *copy = NULL;
        return success;
    }
    Skew_node *new_node = create_skew_node(copy_data(to_copy->data));
    if(!new_node){
        free_skew_node(to_copy);
        return error_allocation;
    }
    Status status = copy_skew_node(&new_node->right, to_copy->right);
    if(status != success){
        free_skew_node(to_copy);
        free_skew_node(*copy);
        return status;
    }
    status = copy_skew_node(&new_node->left, to_copy->left);
    if(status != success){
        free_skew_node(to_copy);
        free_skew_node(*copy);
        return status;
    }
    *copy = new_node;
    return success;
}

Status merge_skew_heap(Skew_heap **result, Skew_heap **heap_1, Skew_heap **heap_2){
    Skew_heap *heap_1_copy, *heap_2_copy;
    Status status = copy_skew_heap(&heap_1_copy, *heap_1);
    if(status != success){
        free_skew_heap(heap_1);
        free_skew_heap(heap_2);
        return status;
    }
    status = copy_skew_heap(&heap_2_copy, *heap_2);
    if(status != success){
        free_skew_heap(heap_1);
        free_skew_heap(heap_2);
        return status;
    }
    return meld_skew_heap(result, &heap_1_copy, &heap_2_copy);
}

Status meld_skew_heap(Skew_heap **result, Skew_heap **first, Skew_heap **second){
    if(!(*first) || !(*second) || *first == *second){
        return wrong_argument;
    }

    Skew_heap *tmp = create_skew_heap();
    if(!tmp){
        free_skew_heap(first);
        free_skew_heap(second);
        return error_allocation;
    }
    tmp->root = merge_skew_node((*second)->root, (*first)->root);
    tmp->size = (*first)->size + (*second)->size;

    (*first)->root = (*second)->root = NULL;
    (*first)->size = (*second)->size = 0;
    free_skew_heap(first);
    free_skew_heap(second);

    *result = tmp;
    return success;
}

void free_skew_heap(Skew_heap **heap){
    if(!(*heap)){
        return;
    }
    if((*heap)->root){
        free_skew_node((*heap)->root);
    }
    free(*heap);
    *heap = NULL;
}

void free_skew_node(Skew_node *node){
    if(!node){
        return;
    }
    free_skew_node(node->left);
    free_skew_node(node->right);

    if(node->data){
        free_data(node->data);
    }
    free(node);
    node = NULL;
}