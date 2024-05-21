#include "trie.h"

Trie *create_trie(){
    Trie *tree = (Trie*)malloc(sizeof(Trie));
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
    node->end = 0;
    return node;
}

Status insert_trie(Trie *tree, char *key, unsigned int value){
    if(!tree){
        return wrong_argument;
    }

    int size = strlen(key);
    Trie_node *cur = tree->root;
    for(int i = 0; i < size; i++){
        int index = key[i] - '0';
        if(isdigit(key[i])){
            index = key[i] - '0';
        }
        else if(key[i] == '_'){
            index = SIZE - 1;
        }
        else{
            if(isupper(key[i])){
                index = key[i] - 'A' + 10;
            }
            else{
                index = key[i] - 'a' + 10;
            }
        }
        if(!cur->children[index]){
            Trie_node *node = create_trie_node();
            if(!node){
                return error_allocation;
            }
            cur->children[index] = node;
        }
        cur = cur->children[index];
    }
    cur->value = value;
    cur->end = 1;
    return success;
}

Status search_trie(Trie *tree, char *key, Trie_node **res){
    if(!tree){
        return wrong_argument;
    }

    int size = strlen(key);
    Trie_node *cur = tree->root;
    for(int i = 0; i < size; i++){
        int index = key[i] - '0';
        if(isdigit(key[i])){
            index = key[i] - '0';
        }
        else if(key[i] == '_'){
            index = SIZE - 1;
        }
        else{
            if(isupper(key[i])){
                index = key[i] - 'A' + 10;
            }
            else{
                index = key[i] - 'a' + 10;
            }
        }
        if(!cur->children[index]){
            return data_not_found;
        }
        cur = cur->children[index];
    }
    if(!cur->end){
        return data_not_found;
    }
    *res = cur;
    return success;
}

void free_trie(Trie *tree){
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
    free(node);
    node = NULL;
}