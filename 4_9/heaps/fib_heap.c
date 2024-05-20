#include "fib_heap.h"

Fib_heap *create_fib_heap(){
    Fib_heap * heap = (Fib_heap*)malloc(sizeof(Fib_heap));
    if(!heap){
        return NULL;
    }
    heap->root = NULL;
    heap->count = 0;
    return heap;
}

Fib_node *create_fib_node(Data *data){
    Fib_node *node = (Fib_node*)malloc(sizeof(Fib_node));
    if(!node){
        return NULL;
    }
    node->left = node->right = node->child = NULL;
    node->data = data;
    node->rank = 0;
    return node;
}

Status insert_fib_heap(Fib_heap **heap, Data *data){
    if(!(*heap)){
        return wrong_argument;
    }
    Fib_node *node = create_fib_node(data);
    if(!node){
        free_fib_heap(heap);
        return error_allocation;
    }
    if(!(*heap)->root){
        node->left = node->right = node;
        (*heap)->root = node;
    }
    else{
        node->left = (*heap)->root->left;
        node->right = (*heap)->root;
        node->left->right = node;
        node->right->left = node;
        if(compare_data(node->data, (*heap)->root->data) > 0){
                (*heap)->root = node;
            }
    }
    (*heap)->count++;
    return success;
}

Status delete_max_fib_heap(Fib_heap **heap, Data **result){
    if(!(*heap) || !(*heap)->root){
        free_fib_heap(heap);
        return wrong_argument;
    }
    
    Fib_node *max = (*heap)->root;
    *result = (*heap)->root->data;
    
    if (max->right == max && !max->child) {
        (*heap)->root = NULL;
    } 
    else if(max->right == max){
        (*heap)->root = (*heap)->root->child;
    }
    else if(!max->child){
        (*heap)->root->left->right = (*heap)->root->right;
        (*heap)->root->right->left = (*heap)->root->left;
        (*heap)->root = (*heap)->root->left;
    }
    else{
        (*heap)->root->left->right = (*heap)->root->child;
        (*heap)->root->right->left = (*heap)->root->child->left;
        (*heap)->root->left->right->left = (*heap)->root->left;
        (*heap)->root->right->left->right = (*heap)->root->right;
        (*heap)->root = (*heap)->root->left;
    }

    free(max);
    max = NULL;
    (*heap)->count--;
    return consolidate_fib_heap(heap);
}

Status merge_fib_node(Fib_heap **heap, Fib_node *first, Fib_node *second, Fib_node **res){
    if(!first || !second || !(*heap)){
        return wrong_argument;
    }

    if(first == second || first->rank != second->rank){
        return wrong_argument;
    }

    if(compare_data(first->data, second->data) < 0){
        Fib_node *tmp = first;
        first = second;
        second = tmp;
    }

    if(first->child){
        second->right = first->child;
        second->left = first->child->left;
        second->left->right = second;
        second->right->left = second;
    }
    else{
        second->left = second;
        second->right = second;
    }
    first->child = second;
    first->rank++;

    *res = first;
    return success;
}

Status consolidate_fib_heap(Fib_heap **heap){
    if(!(*heap)){
        return wrong_argument;
    }
    if(!(*heap)->root || (*heap)->root->right == (*heap)->root){
        return success;
    }

    Fib_node *cur = (*heap)->root;
    int max_rank = log2(1.0 * (*heap)->count) + 3;
    Fib_node **arr = (Fib_node**)calloc(max_rank, sizeof(Fib_node*));
    if(!arr){
        free_fib_heap(heap);
        return error_allocation;
    }
    
    (*heap)->root->left->right = NULL;
    while(cur){
        Fib_node *next = cur->right;
        if(cur->right == cur){
            cur->right = NULL;
        }
        int rank = cur->rank;
        while(arr[rank]){
            Fib_node *tmp = arr[rank];
            arr[rank] = NULL;
            Status status = merge_fib_node(heap, cur, tmp, &cur);
            if(status != success){
                free_fib_heap(heap);
                return status;
            } 
            arr[max_rank - 1] = cur;
        }
        arr[rank] = cur;
        cur = next;
    }
    Fib_node *top, *start, *prev;
    for(int i = 0; i < max_rank; i++){
        if(!arr[i]){
            continue;
        }
        start = start ? start : arr[i];
        top = top ? top : arr[i];
        if(compare_data(arr[i]->data, top->data) > 0){
            top = arr[i];
        }
        if(prev){
            prev->right = arr[i];
        }
        arr[i]->left = prev;
        prev = arr[i];
    }
    prev->right = start;
    start->left = prev;
    (*heap)->root = top;
    free(arr);

    return success;
}

Status find_max_fib_heap(Fib_heap **heap, Data **data){
    if(!(*heap)){
        return wrong_argument;
    }
    *data = (*heap)->root->data;
    return success;
}

Status merge_fib_heap(Fib_heap **result, Fib_heap **heap_1, Fib_heap **heap_2){
    Fib_heap *copy_1, *copy_2;
    Status status = copy_fib_heap(&copy_1, *heap_1);
    if(status != success){
        free_fib_heap(heap_1);
        free_fib_heap(heap_2);
        return status;
    }
    status = copy_fib_heap(&copy_2, *heap_2);
    if(status != success){
        free_fib_heap(heap_1);
        free_fib_heap(heap_2);
        return status;
    }
    return meld_fib_heap(result, &copy_1, &copy_2);
}

Status copy_fib_heap(Fib_heap **copy, Fib_heap *to_copy){
    if(*copy){
        free_fib_heap(&to_copy);
        return wrong_argument;
    }
    if(!to_copy){
        *copy = NULL;
        return success;
    }

    *copy = create_fib_heap();
    if(!copy){
        free_fib_heap(&to_copy);
        return error_allocation;
    }
    Status status = copy_fib_node(&(*copy)->root, to_copy->root);
    if(status != success){
        free_fib_heap(&to_copy);
        return status;
    }
    return success;
}

Status copy_fib_node(Fib_node **copy, Fib_node *to_copy){
    if(*copy){
        return wrong_argument;
    }
    if(!to_copy){
        return success;
    }

    *copy = create_fib_node(copy_data(to_copy->data));
    if(!(*copy)){
        free_fib_node(to_copy);
        return error_allocation;
    }
    (*copy)->rank = to_copy->rank;

    Status status = copy_fib_node(&(*copy)->child, to_copy->child);
    if(status != success){
        free_fib_node(*copy);
        free_fib_node(to_copy);
    }
    status = copy_fib_node(&(*copy)->right, to_copy->right);
    if(status != success){
        free_fib_node(*copy);
        free_fib_node(to_copy);
    }

    if((*copy)->right){
        (*copy)->left = (*copy)->right->left;
        (*copy)->left->right = (*copy);
        (*copy)->right->left = (*copy);
    }
    else{
        (*copy)->left = (*copy);
    }
    return success;
}

Status meld_fib_heap(Fib_heap **result, Fib_heap **heap_1, Fib_heap **heap_2){
    if(*result){
        return wrong_argument;
    }
    if(!(*heap_1) && !(*heap_2)){
        return success;
    }
    if(!(*heap_1)){
        *result = *heap_2;
        *heap_2 = NULL;
        return success;
    }
    if(!(*heap_2)){
        *result = *heap_1;
        *heap_1 = NULL;
        return success;
    }
    *result = *heap_1;
    Fib_node *tmp = (*result)->root->right;
    (*result)->root->right = (*heap_2)->root;
    (*heap_2)->root->left = (*result)->root;
    tmp->left = (*heap_2)->root;
    (*heap_2)->root->right = tmp;

    if(compare_data((*result)->root->data, (*heap_2)->root->data) < 0){
        (*result)->root = (*heap_2)->root;
    }    
    
    free(*heap_2);
    *heap_1 = NULL;
    *heap_2 = NULL;
    return success;
}

void free_fib_heap(Fib_heap **heap){
    if(!(*heap)){
        return;
    }
    if((*heap)->root){
        free_fib_node((*heap)->root);
    }
    free(*heap);
    *heap = NULL;
}

void free_fib_node(Fib_node *node){
    if(!node){
        return;
    }
    if(node->left){
        node->left->right = NULL;
    }
    free_fib_node(node->child);
    free_fib_node(node->right);
    if(node->data){
        free_data(node->data);
    }
    node->child = node->left = node->right = NULL;
    free(node);
    node = NULL;
}