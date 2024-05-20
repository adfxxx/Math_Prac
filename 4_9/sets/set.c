#include <stdio.h>
#include <stdlib.h>
#include "set.h"
#include "dynamic_array.h"
#include "bst.h"
#include "hash_set.h"
#include "trie.h"

Status create_set(Set *set, Set_type type){
    if(!set){
        return wrong_argument;
    }
    
    set->dep = NULL;
    void *dep = NULL;   
    switch(type){
        case Array:
            dep = create_array();
            if(!dep){
                free(set->dep);
                return error_allocation;
            }
            set->find = (Status (*)(void *, int, Department **)) search_array;
            set->free = (void (*)(void*)) free_array;
            set->insert = (Status(*)(void *, int, Department *)) insert_array;
            break;
        case Bst:
            dep = create_bst();
            if(!dep){
                free(set->dep);
                return error_allocation;
            }
            set->find = (Status (*)(void *, int, Department **)) search_bst;
            set->free = (void (*)(void*)) free_bst;
            set->insert = (Status(*)(void*, int, Department *)) insert_bst;
            break;
        case Hash:
            dep = create_hash();
            if(!dep){
                free(set->dep);
                return error_allocation;
            } 
            set->find = (Status (*)(void *, int, Department **)) search_hash;
            set->free = (void (*)(void*)) free_hash;
            set->insert = (Status(*)(void*, int, Department *)) insert_hash;
            break;
        case Trie:
            dep = create_trie();
            if(!dep){
                free(set->dep);
                return error_allocation;
            }  
            set->find = (Status (*)(void *, int, Department **)) search_trie;
            set->free = (void (*)(void*)) free_trie;
            set->insert = (Status(*)(void*, int, Department *)) insert_trie;
            break;
        default:
            break;
    }

    set->dep = dep;
    return success;
}

Status insert_set(Set *set, Data *data){
    Department *dep = NULL;
    Status status = set->find(set->dep, data->dep_id, &dep);
    if(status != success){
        return status;
    }
    status = insert_req(dep, data);
    if(status != success){
        return status;
    }
    return success;
}

Status free_set(Set *set){
    if(!set){
        return wrong_argument;
    }

    set->free(set->dep);
    free(set->dep);
    return success;
}
