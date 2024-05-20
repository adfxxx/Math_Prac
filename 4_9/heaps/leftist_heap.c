#include "leftist_heap.h"

Leftist_heap *create_leftist_heap(){
    Leftist_heap * heap = (Leftist_heap*)malloc(sizeof(Leftist_heap));
    if(!heap){
        return NULL;
    }
    heap->root = NULL;
    heap->size = 0;
    return heap;
}

Leftist_node *create_leftist_node(Data *data){
    Leftist_node *node = (Leftist_node*)malloc(sizeof(Leftist_node));
    if(!node){
        free_data(data);
        return NULL;
    }
    node->data = data;
    node->dist = 0;
    node->left = node->right = NULL;
    return node;
}

Status insert_leftist_heap(Leftist_heap **heap, Data *data){
    if(!(*heap)){
        return wrong_argument;
    }
    Leftist_node *node = create_leftist_node(data);
    if(!node){
        free_leftist_heap(heap);
        free_data(data);
        return error_allocation;
    }

    Leftist_heap *tmp = create_leftist_heap();
    if(!tmp){
        return error_allocation;
    }
    tmp->root = node;
    tmp->size = 1;

    Leftist_heap *new_heap = NULL;
    Status status = meld_leftist_heap(&new_heap, heap, &tmp);
    if(status != success){
        free_leftist_heap(heap);
        free_leftist_heap(&tmp);
        free_data(data);
        free_leftist_node(node);
        return error_allocation;
    }
    *heap = new_heap;
    return success;
}

Status delete_max_leftist_heap(Leftist_heap **heap, Data **data){
    if(!heap){
        return wrong_argument;
    }
    if(!(*heap)->root){
        return success;
    }

    *data = (*heap)->root->data;
    Leftist_node *root = (*heap)->root;
    (*heap)->root = merge_leftist_node((*heap)->root->right, (*heap)->root->left);

    free(root);
    (*heap)->size--;
    return success;
}

Status find_max_leftist_heap(Leftist_heap **heap, Data **data){
    if(!heap){
        return wrong_argument;
    }
    if(!(*heap)->root){
        return success;
    }
    *data = (*heap)->root->data;
    return success;
}

Leftist_node *merge_leftist_node(Leftist_node *first, Leftist_node *second){
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
        Leftist_node *tmp = first;
        first = second;
        second = tmp;
    }

    first->right = merge_leftist_node(first->right, second);
    if(first->right && (first->left || first->left->dist < first->right->dist)){
        Leftist_node *tmp = first->left;
        first->left = first->right;
        first->right = tmp;
    }
    first->dist = first->right == NULL ? 0 : first->right->dist + 1;
    return first;
}

Status copy_leftist_heap(Leftist_heap **copy, Leftist_heap *to_copy){
    if(*copy || !to_copy){
        return wrong_argument;
    }

    Status status = copy_leftist_node(&(*copy)->root, to_copy->root);
    if(status != success){
        free_leftist_heap(copy);
        free_leftist_heap(&to_copy);
        return status;
    }

    (*copy)->size = to_copy->size;
    return success;
}

Status copy_leftist_node(Leftist_node **copy, Leftist_node *to_copy){
    if(*copy){
        free_leftist_node(to_copy);
        return wrong_argument;
    }
    if(!to_copy){
        *copy = NULL;
        return success;
    }
    Leftist_node *new_node = create_leftist_node(copy_data(to_copy->data));
    if(!new_node){
        free_leftist_node(to_copy);
        return error_allocation;
    }
    new_node->dist = to_copy->dist;
    Status status = copy_leftist_node(&new_node->right, to_copy->right);
    if(status != success){
        free_leftist_node(to_copy);
        free_leftist_node(*copy);
        return status;
    }
    status = copy_leftist_node(&new_node->left, to_copy->left);
    if(status != success){
        free_leftist_node(to_copy);
        free_leftist_node(*copy);
        return status;
    }
    *copy = new_node;
    return success;
}

Status merge_leftist_heap(Leftist_heap **result, Leftist_heap **heap_1, Leftist_heap **heap_2){
    Leftist_heap *heap_1_copy, *heap_2_copy;
    Status status = copy_leftist_heap(&heap_1_copy, *heap_1);
    if(status != success){
        free_leftist_heap(heap_1);
        free_leftist_heap(heap_2);
        return status;
    }
    status = copy_leftist_heap(&heap_2_copy, *heap_2);
    if(status != success){
        free_leftist_heap(heap_1);
        free_leftist_heap(heap_2);
        return status;
    }
    return meld_leftist_heap(result, &heap_1_copy, &heap_2_copy);
}

Status meld_leftist_heap(Leftist_heap **result, Leftist_heap **first, Leftist_heap **second){
    if(!(*first) || !(*second) || *first == *second){
        return wrong_argument;
    }

    Leftist_heap *tmp = create_leftist_heap();
    if(!tmp){
        free_leftist_heap(first);
        free_leftist_heap(second);
        return error_allocation;
    }
    tmp->root = merge_leftist_node((*second)->root, (*first)->root);
    tmp->size = (*first)->size + (*second)->size;

    (*first)->root = (*second)->root = NULL;
    (*first)->size = (*second)->size = 0;
    free_leftist_heap(first);
    free_leftist_heap(second);

    *result = tmp;
    return success;
}

void free_leftist_heap(Leftist_heap **heap){
    if(!(*heap)){
        return;
    }
    if((*heap)->root){
        free_leftist_node((*heap)->root);
    }
    free(*heap);
    *heap = NULL;
}

void free_leftist_node(Leftist_node *node){
    if(!node){
        return;
    }
    free_leftist_node(node->left);
    free_leftist_node(node->right);

    if(node->data){
        free_data(node->data);
    }
    free(node);
    node = NULL;
}