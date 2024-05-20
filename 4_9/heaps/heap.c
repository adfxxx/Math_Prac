#include "heap.h"
#include "binary_heap.h"
#include "binomial_heap.h"
#include "fib_heap.h"
#include "leftist_heap.h"
#include "skew_heap.h"
#include "treap.h"

Status create_heap(Heap *h, Heap_type type){
    if (!h){
        return wrong_argument;
    }
    h->type = type;
    void *dep = NULL;
    switch(type){
        case Binary:
            dep = create_binary_heap();
            if(!dep){
                free(h);
                return error_allocation;
            }
            h->free = (void (*)(void**)) free_binary_heap;
            h->insert = (Status(*)(void**, Data*)) insert_binary_heap;
            h->find_max = (Status (*)(void**, Data**)) find_max_binary_heap;
            h->delete_max = (Status (*) (void**, Data**)) delete_max_binary_heap;
            h->merge = (Status (*)(void**, void**,void**)) merge_binary_heap;
            h->meld = (Status (*)(void**, void**,void**)) meld_binary_heap;
            break;
        case Binomial:
            dep = create_binomial_heap();
            if(!dep){
                free(h);
                return error_allocation;
            }
            h->free = (void (*)(void**)) free_binomial_heap;
            h->insert = (Status(*)(void**, Data*)) insert_binomial_heap;
            h->find_max = (Status (*)(void**, Data**)) find_max_binomial_heap;
            h->delete_max = (Status (*) (void**, Data**)) delete_max_binomial_heap;
            h->merge = (Status (*)(void**, void**,void**)) merge_binomial_heap;
            h->meld = (Status (*)(void**, void**,void**)) meld_binomial_heap;
            break;
        case Fibonacci:
            dep = create_fib_heap();
            if(!dep){
                free(h);
                return error_allocation;
            }
            h->free = (void (*)(void**)) free_fib_heap;
            h->insert = (Status(*)(void**, Data*)) insert_fib_heap;
            h->find_max = (Status (*)(void**, Data**)) find_max_fib_heap;
            h->delete_max = (Status (*) (void**, Data**)) delete_max_fib_heap;
            h->merge = (Status (*)(void**, void**,void**)) merge_fib_heap;
            h->meld = (Status (*)(void**, void**,void**)) meld_fib_heap;
            break;
        case Leftist:
            dep = create_leftist_heap();
            if(!dep){
                free(h);
                return error_allocation;
            } 
            h->free = (void (*)(void**)) free_leftist_heap;
            h->insert = (Status(*)(void**, Data*)) insert_leftist_heap;
            h->find_max = (Status (*)(void**, Data**)) find_max_leftist_heap;
            h->delete_max = (Status (*) (void**, Data**)) delete_max_leftist_heap;
            h->merge = (Status (*)(void**, void**,void**)) merge_leftist_heap;
            h->meld = (Status (*)(void**, void**,void**)) meld_leftist_heap;
            break;
        case Skew:
            dep = create_skew_heap();
            if(!dep){
                free(h);
                return error_allocation;
            }  
            h->free = (void (*)(void**)) free_skew_heap;
            h->insert = (Status(*)(void**, Data*)) insert_skew_heap;
            h->find_max = (Status (*)(void**, Data**)) find_max_skew_heap;
            h->delete_max = (Status (*) (void**, Data**)) delete_max_skew_heap;
            h->merge = (Status (*)(void**, void**,void**)) merge_skew_heap;
            h->meld = (Status (*)(void**, void**,void**)) meld_skew_heap;
            break;
        case Treap:
            dep = create_treap();
            if(!dep){
                free(h);
                return error_allocation;
            }   
            h->free = (void (*)(void**)) free_treap;
            h->insert = (Status(*)(void**, Data*)) insert_treap;
            h->find_max = (Status (*)(void**, Data**)) find_max_treap;
            h->delete_max = (Status (*) (void**, Data**)) delete_max_treap;
            h->merge = (Status (*)(void**, void**,void**)) merge_treap;
            h->meld = (Status (*)(void**, void**,void**)) meld_treap;
            break;
    }

    h->dep = dep;
    return success;
}

void free_heap(Heap *h){
    if(!h){
        return;
    }
    if(h->dep){
        h->free(h->dep);
        free(h->dep);
    }
}