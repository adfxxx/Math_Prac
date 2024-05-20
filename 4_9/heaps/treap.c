#include "treap.h"

Treap_heap *create_treap(){
    Treap_heap *heap = (Treap_heap*)malloc(sizeof(Treap_heap));
    if(!heap){
        return NULL;
    }
    heap->root = NULL;
    heap->size = 0;
    return heap;
}

Treap_node *create_treap_node(Data *data){
    Treap_node *node = (Treap_node*)malloc(sizeof(Treap_node));
    if(!node){
        free_data(data);
        return NULL;
    }
    node->data = data;
    node->left = node->right = NULL;

    node->prior = rand();
    return node;
}

Status insert_treap(Treap_heap **heap, Data *data){
    if(!(*heap)){
        return wrong_argument;
    }
    Treap_node *node = create_treap_node(data);
    if(!node){
        free_treap(heap);
        free_data(data);
        return error_allocation;
    }
    Treap_node *less, *greater;
    Status status = split_treap((*heap)->root, data, &less, &greater);
    if(status != success){
        free_treap_node(node);
        return status;
    }

    (*heap)->root = merge_treap_node(merge_treap_node(less, node), greater);
    (*heap)->size++;
    return success;
}

Status split_treap(Treap_node *node, Data *data, Treap_node **first, Treap_node **second){
    if(!node){
        return success;
    }
    Status status;
    if(compare_data(node->data, data) > 0){
        status = split_treap(node->right, data, &node->right, second);
        if(status != success){
            return status;
        }
        *first = node;
    }
    else{
        status = split_treap(node->left, data, first, &node->left);
        if(status != success){
            return status;
        }
        *second = node;
    }
    return success;
}

Status delete_max_treap(Treap_heap **heap, Data **data){
    if(!(*heap)){
        return wrong_argument;
    }
    if(!(*heap)->root){
        return success;
    }

    Treap_node *node = (*heap)->root;
    if(!node->right){
        *data = node->data;
        (*heap)->root = node->left;
        free(node);
        return success;
    }

    Treap_node *prev = node;
    node = node->right;
    while(node->right){
        prev = node;
        node = node->right;
    }
    *data = node->data;
    prev->right = node->left;
    free(node);
    (*heap)->size--;
    return success;
}

Status find_max_treap(Treap_heap **heap, Data **data){
    if(!(*heap)){
        return wrong_argument;
    }
    if(!(*heap)->root){
        return success;
    }
    Treap_node *node = (*heap)->root;
    while(node->right){
        node = node->right;
    }
    *data = node->data;
    return success;
}

Status copy_treap(Treap_heap **copy, Treap_heap *to_copy){
    if(*copy || !to_copy){
        return wrong_argument;
    }

    Status status = copy_treap_node(&(*copy)->root, to_copy->root);
    if(status != success){
        free_treap(copy);
        free_treap(&to_copy);
        return status;
    }

    (*copy)->size = to_copy->size;
    return success;
}

Status copy_treap_node (Treap_node **copy, Treap_node *to_copy){
    if(*copy){
        free_treap_node(to_copy);
        return wrong_argument;
    }
    if(!to_copy){
        *copy = NULL;
        return success;
    }
    Treap_node *new_node = create_treap_node(copy_data(to_copy->data));
    if(!new_node){
        free_treap_node(to_copy);
        return error_allocation;
    }
    Status status = copy_treap_node(&new_node->right, to_copy->right);
    if(status != success){
        free_treap_node(to_copy);
        free_treap_node(*copy);
        return status;
    }
    status = copy_treap_node(&new_node->left, to_copy->left);
    if(status != success){
        free_treap_node(to_copy);
        free_treap_node(*copy);
        return status;
    }
    *copy = new_node;
    return success;
}

Status merge_treap(Treap_heap **result, Treap_heap **heap_1, Treap_heap **heap_2){
    Treap_heap *heap_1_copy, *heap_2_copy;

    Status status = copy_treap(&heap_1_copy, *heap_1);
    if(status != success){
        free_treap(heap_1);
        free_treap(heap_2);
        return status;
    }
    status = copy_treap(&heap_2_copy, *heap_2);
    if(status != success){
        free_treap(heap_1);
        free_treap(heap_2);
        return status;
    }
    return meld_treap(result, &heap_1_copy, &heap_2_copy);
}

Treap_node *merge_treap_node(Treap_node *first, Treap_node *second){
    if(!first && !second){
        return NULL;
    }
    if(!first){
        return second;
    }
    if(!second){
        return first;
    }

    if(first->prior > second->prior){
        first->right = merge_treap_node(first->right, second);
        return first;
    }
    second->left = merge_treap_node(first, second->left);
    return second;
}

Status meld_treap(Treap_heap **result, Treap_heap **heap_1, Treap_heap **heap_2){
    if(!heap_1 && !heap_2){
        return wrong_argument;
    }

    if(!((*heap_1)->root) && !((*heap_2)->root)){
        return success;
    }

    if(!(*result)){
        free_treap(heap_1);
        free_treap(heap_2);
        return error_allocation;
    }

    (*result)->root = merge_treap_node((*heap_1)->root, (*heap_2)->root);
    (*heap_1)->root = (*heap_2)->root = NULL;
    free_treap(heap_1);
    free_treap(heap_2);
    return success;
}

void free_treap(Treap_heap **heap){
    if(!(*heap)){
        return;
    }
    if((*heap)->root){
        free_treap_node((*heap)->root);
    }
    free(*heap);
    *heap = NULL;
}

void free_treap_node(Treap_node *node){
    if(!node){
        return;
    }
    free_treap_node(node->left);
    free_treap_node(node->right);

    if(node->data){
        free_data(node->data);
    }
    free(node);
    node = NULL;
}
