#include "hash_set.h"
#define HASH 37

Hash_set *create_hash(){
    Hash_set *set = (Hash_set*)malloc(sizeof(Hash_set));
    if(!set){
        return NULL;
    }

    set->size = 5;
    set->max_size = set->min_size = 0;
    set->elems = create_hash_elem(set->size);
    if(!set->elems){
        free(set);
        return NULL;
    }
    return set;
}

Hash_elem **create_hash_elem(int size){
    Hash_elem **elems = (Hash_elem**)calloc(size, sizeof(Hash_elem*));
    if(!elems){
        return NULL;
    }
    return elems;
}

Hash_elem *create_hash_elem_in(int key, Department *dep){
    Hash_elem *elem = (Hash_elem*)malloc(sizeof(Hash_elem));
    if(!elem){
        return NULL;
    }
    elem->dep = dep;
    elem->next_count = 0;
    elem->next = NULL;
    elem->key = key;
    return elem;
}

Status insert_hash(Hash_set *set, int key, Department *dep){
    if(!set || !dep){
        return wrong_argument;
    }

    Hash_elem *elem = create_hash_elem_in(key, dep);
    if(!elem){
        return error_allocation;
    }

    int index = hash_function(key, set->size);
    elem->next = set->elems[index];
    set->elems[index] = elem;
    if(elem->next){
        elem->next_count = elem->next->next_count;
    }
    if(set->max_size < elem->next_count){
        set->max_size = elem->next_count;
    }
    if(set->min_size > elem->next_count){
        set->max_size = elem->next_count;
    }
    if(set->max_size > 2*(set->min_size ? set->min_size : 1)){
        return rebuild_hash(set);
    }
    return success;
}

Status search_hash(Hash_set *set, int key, Department **res){
    if(!set){
        return wrong_argument;
    }

    int index = hash_function(key, set->size);
    Hash_elem *cur = set->elems[index];
    while(cur && cur->key != key){
        cur = cur->next;
    }
    if(!cur){
        return data_not_found;
    }
    *res = cur->dep;
    return success;
}

void free_hash(Hash_set *set){
    if(!set){
        return;
    }
    free_hash_elems(set->elems, set->size);
    free(set);
    set = NULL;
}

void free_hash_elems(Hash_elem **elems, int size){
    if(!elems){
        return;
    }

    for(int i = 0; i < size; i++){
        free_hash_elem_in(elems[i]);
    }
    free(elems);
    elems = NULL;
}

void free_hash_elem_in(Hash_elem *elem){
    if(!elem){
        return;
    }

    free_hash_elem_in(elem->next);
    elem->next = NULL;
    free_dep(elem->dep);
    free(elem);
    elem = NULL;
}

Status rebuild_hash(Hash_set *set){
    int prev_size = set->size;
    set->size = get_prime_num(set->size);
    Status status = change_elem_size_hash(&set->elems, prev_size, set->size);
    if(status != success){
        return status;
    }
    if(!set->elems){
        return error_allocation;
    }

    for(int i = 0; i < prev_size; i++){
        Hash_elem *cur = set->elems[i];
        Hash_elem *prev = NULL;
        Hash_elem *first = cur;
        while(cur){
            Hash_elem *next = cur->next;

            int index = hash_function(cur->key, set->size);
            if(index == i){
                continue;
            }
            if(prev){
                prev->next = next;
                low_chain_size(first, next);
            }

            cur->next = set->elems[index];
            set->elems[index] = cur;

            if(cur->next){
                cur->next_count = cur->next->next_count;
            }
            else{
                cur->next_count = 0;
            }

            if(set->max_size < cur->next_count){
                set->max_size = cur->next_count;
            }
            if(set->min_size > cur->next_count){
                set->max_size = cur->next_count;
            }
        }
    }
    if(set->max_size > 2*(set->min_size ? set->min_size : 1)){
        return rebuild_hash(set);
    }
}

void low_chain_size(Hash_elem *from, Hash_elem *to){
    Hash_elem *cur = from;
    while(cur != to){
        cur->next_count--;
        cur = cur->next;
    }
}

Status change_elem_size_hash(Hash_elem ***elems, int old_size, int new_size){
    Hash_elem **tmp = (Hash_elem **)realloc(*elems, new_size*sizeof(Hash_elem*));
    if(!tmp){
        free_hash_elems(*elems, old_size);
        return error_allocation;
    }
    *elems = tmp;
    return success;
}

int get_prime_num(int size){
    while(1){
        int num = 1;
        size++;
        for(int i = 2; i <= size; i++){
            if(size % i == 0){
                num++;
            }
        }
        if(num == 2){
            return size;
        }
    }
}

int hash_function(int key, int size){
    return key%size;
}