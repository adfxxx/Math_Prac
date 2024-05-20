#include "trie.h"

Trie_set *create_trie(){
    Trie_set *tree = (Trie_set*)malloc(sizeof(Trie_set));
    if(!tree){
        return NULL;
    }
    tree->root = create_trie_node();
    if(!tree->root){
        free(tree);
        return NULL;
    }
    return tree;
}

Trie_node *create_trie_node(){
    Trie_node *node = (Trie_node*)malloc(sizeof(Trie_node));
    if(!node){
        return NULL;
    }
    for(int i = 0; i < SIZE; i++){
        node->children[i] = NULL;
    }
    return node;
}

Status insert_trie(Trie_set *tree, int key, Department *dep){
    if(!tree || !dep){
        return wrong_argument;
    }

    int size = get_digits_amount(key);
    char str_key[size];
    sprintf(str_key, "%d", key);

    Trie_node *cur = tree->root;
    for(int i = 0; i < size; i++){
        int digit = str_key[i] - '0';
        if(!cur->children[digit]){
            Trie_node *node = create_trie_node();
            if(!node){
                return error_allocation;
            }
            cur->children[digit] = node;
        }
        cur = cur->children[digit];
    }
    cur->dep = dep;
    return success;
}

Status search_trie(Trie_set *tree, int key, Department **res){
    if(!tree){
        return wrong_argument;
    }

    int size = get_digits_amount(key);
    char str_key[size];
    sprintf(str_key, "%d", key);

    Trie_node *cur = tree->root;
    for(int i = 0; i < size; i++){
        int digit = str_key[i] - '0';
        if(!cur->children[digit]){
            return data_not_found;
        }
        cur = cur->children[digit];
    }
    if(!cur->dep){
        return data_not_found;
    }
    *res = cur->dep;
    return success;
}

void free_trie(Trie_set *tree){
    if(!tree){
        return;
    }
    free_trie_node(tree->root);
    free(tree);
    tree = NULL;
}

void free_trie_node(Trie_node *node){
    if(!node){
        return;
    }

    for(int i = 0; i < SIZE; i++){
        free_trie_node(node->children[i]);
    }
    free_dep(node->dep);
    free(node);
    node = NULL;
}

int get_digits_amount(int num){
    if(!num/10){
        return 1;
    }
    int res = 0;
    while(num > 0){
        num/=10;
        res++;
    }
    return res;
}