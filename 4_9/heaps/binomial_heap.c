#include "binomial_heap.h"

Binomial_heap *create_binomial_heap(){
    Binomial_heap * heap = (Binomial_heap*)malloc(sizeof(Binomial_heap));
    if(!heap){
        return NULL;
    }
    heap->root = NULL;
    heap->max = NULL;
    return heap;
}

Binomial_node *create_binomial_node(Data *data){
    Binomial_node *node = (Binomial_node*)malloc(sizeof(Binomial_node));
    if(!node){
        return NULL;
    }
    node->brother = NULL;
    node->child = NULL;
    node->data = data;
    node->rank = 0;
    return node;
}

Status insert_binomial_heap(Binomial_heap **heap, Data *data){
    if(!(*heap)){
        return wrong_argument;
    }
    Binomial_node *node = create_binomial_node(data);
    if(!node){
        free_binomial_heap(heap);
        return error_allocation;
    }
    Binomial_heap *add = create_binomial_heap();
    if(!add){
        free_binomial_heap(heap);
        free_binomial_node(node);
        return error_allocation;
    }
    add->root = node;
    add->max = node;
    Binomial_heap *new = NULL;
    Binomial_node *max = (*heap)->max;
    meld_binomial_heap(&new, heap, &add);
    *heap = new;
    (*heap)->max = max;
    if(!max || (max && compare_data(max->data, node->data) < 0)){
        (*heap)->max = node;
    }
    return success;
}

Status delete_max_binomial_heap(Binomial_heap **heap, Data **data){
    if(find_max_binomial_heap(heap, data) != success){
        return wrong_argument;
    }
    *data = (*heap)->max->data;
    Binomial_node *max = (*heap)->max;
    Binomial_node *cur = (*heap)->root;
    Binomial_node *prev = NULL;
    while(cur && cur != max){
        prev = cur;
        cur = cur->brother;
    }

    if(!prev){
        (*heap)->root = cur->brother;
    }
    else{
        prev->brother = max->brother;
    }

    Binomial_heap *tmp = create_binomial_heap(); 
    if(!tmp){
        free_binomial_heap(heap);
        return error_allocation;
    }

    Binomial_node *node = max->child;
    Binomial_node *next = node->brother;
    Binomial_node *brother;

    while(next){
        brother = next->brother;
        next->brother = node;
        node->brother = NULL;
        node = next;
        next = brother;
    }
    tmp->root = node;
    Binomial_heap *new_heap = NULL;
    meld_binomial_heap(&new_heap, heap, &tmp);
    *heap = new_heap;
    (*heap)->max = get_max_binomial_heap(heap);

    free(max);
    max = NULL;
    return success;
}

Status find_max_binomial_heap(Binomial_heap **heap, Data **data){
    if(!(*heap)){
        free_binomial_heap(heap);
        return success;
    }
    *data = (*heap)->max->data;
    return success;
}

Binomial_node *get_max_binomial_heap(Binomial_heap **heap){
    Binomial_node *max = NULL;
    Binomial_node *cur = (*heap)->root;
    while(cur){
        if(!max || compare_data(max->data, cur->data) < 0){
            max = cur;
        }
        cur = cur->brother;
    }
    return max;
}

Status merge_binomial_heap(Binomial_heap **result, Binomial_heap **heap_1, Binomial_heap **heap_2){
    if((*result) || !(*heap_1) || !(*heap_2)){
        free_binomial_heap(heap_1);
        free_binomial_heap(heap_2);
        return wrong_argument;
    }
    Binomial_heap *left = copy_binomial_heap(*heap_1);
    Binomial_heap *right = copy_binomial_heap(*heap_2);
    if(!left){
        free_binomial_heap(heap_1);
        free_binomial_heap(heap_2);
        return error_allocation;
    }
    if(!right){
        free_binomial_heap(&left);
        free_binomial_heap(heap_1);
        free_binomial_heap(heap_2);
        return error_allocation;
    }
    Status status = meld_binomial_heap(result, &left, &right);
    if(status != success){
        free_binomial_heap(&left);
        free_binomial_heap(&right);
        return status;
    }
    return success;
}

Status meld_binomial_heap(Binomial_heap **result, Binomial_heap **heap_1, Binomial_heap **heap_2){
    if((*result) || !(*heap_1) || !(*heap_2)){
        free_binomial_heap(heap_1);
        free_binomial_heap(heap_2);
        return wrong_argument;
    }

    Binomial_node *left = (*heap_1)->root;
    Binomial_node *right = (*heap_2)->root;
    Status status;
    if(!left && !right){
        *result = create_binomial_heap();
        if(!result){
            free_binomial_heap(heap_1);
            free_binomial_heap(heap_2);
            return error_allocation;
        }
        return success;
    }

    Binomial_heap *tmp = create_binomial_heap();
    if(!tmp){
        free_binomial_heap(heap_1);
        free_binomial_heap(heap_2);
        return error_allocation;
    }

    if(left && (!right || left->rank < right->rank)){
        tmp->root = left;
        left = left->brother;
    }
    else{
        tmp->root = right;
        right = right->brother;
    }

    Binomial_node *res = tmp->root;
    while(left && right){
        if(left->rank < right->rank){
            res->brother = left;
            left = left->brother;
        }
        else{
            res->brother = right;
            right = right->brother;
        }
        res = res->brother;
    }

    while(left){
        res->brother = left;
        left = left->brother;
        res = res->brother;
    }
    while(right){
        res->brother = right;
        right = right->brother;
        res = res->brother;
    }

    Binomial_node *prev = NULL;
    Binomial_node *cur = tmp->root;
    Binomial_node *next = tmp->root->brother;

    while(next){
        Binomial_node *tmp_cur = next->brother;
        if(cur->rank == next->rank &&(!tmp_cur || tmp_cur->rank != next->rank)){
            if(compare_data(cur->data, next->data) > 0){
                next->brother = cur->child;
                cur->child = next;
            }
            else{
                cur->brother = next->child;
                next->child = cur;
                if(cur == tmp->root){
                    tmp->root = next;
                }
                cur = next;
            }
            if(prev){
                prev->brother = cur;
            }
            cur->rank++;
            cur->brother = tmp_cur;
            next = tmp_cur;
        }
        else{
            prev = cur;
            cur = next;
            next = next->brother;
        }
    }
    (*heap_1)->root = NULL;
    (*heap_2)->root = NULL;
    free_binomial_heap(heap_1);
    free_binomial_heap(heap_2);

    *result = tmp;
    return success;
}

Binomial_heap *copy_binomial_heap(Binomial_heap *heap){
    if(!heap){
        return NULL;
    }


    Binomial_heap *new = create_binomial_heap();
    if(!new){
        return NULL;
    }
    if(copy_binomial_node(&new->root, heap->root) != success){
        return NULL;
    }
    return new;
}

Status copy_binomial_node(Binomial_node **end, Binomial_node *start){
    *end = NULL;
    if(!start){
        return success;
    }
    Binomial_node *tmp = create_binomial_node(copy_data(start->data));
    if(!tmp){
        return error_allocation;
    }
    tmp->rank = start->rank;
    if(copy_binomial_node(&tmp->child, start->child) != success || copy_binomial_node(&tmp->brother, start->brother) != success){
        free_binomial_node(tmp);
        return error_allocation;
    }
    *end = tmp;
    return success;
}

void free_binomial_heap(Binomial_heap **heap){
    if(!(*heap)){
        return;
    }
    if((*heap)->root){
        free_binomial_node((*heap)->root);
    }
    free(*heap);
    *heap = NULL;
}

void free_binomial_node(Binomial_node *node){
    if(!node){
        return;
    }
    free_binomial_node(node->child);
    free_binomial_node(node->brother);
    free_data(node->data);
    free(node);
    node = NULL;
}