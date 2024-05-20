#include "bst.h"

BST *create_bst(){
    BST *tree = (BST*)malloc(sizeof(BST));
    if(!tree){
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

BST_node *create_bst_node(int key, Department *dep){
    BST_node *node = (BST_node*)malloc(sizeof(BST_node));
    if(!node){
        return NULL;
    }

    node->key = key;
    node->dep = dep;
    node->left = node->right = NULL;
    return node;
}

Status insert_bst(BST *tree, int key, Department *dep){
    if(!tree || !dep){
        return wrong_argument;
    }

    if(!tree->root){
        tree->root = create_bst_node(key, dep);
        if(!tree->root){
            return error_allocation;
        }
        return success;
    }

    BST_node *cur = tree->root;
    BST_node *parent = NULL;
    while(cur){
        parent = cur;
        if(key == cur->key){
            return already_exist;
        }
        if(key < cur->key){
            cur = cur->left;
        }
        else{
            cur = cur->right;
        }
    }

    parent->right = create_bst_node(key, dep);
    if(!parent->right){
        return error_allocation;
    }
    return success;
}

Status search_bst(BST *tree, int key, Department **dep){
    if(!tree || *dep){
        return wrong_argument;
    }

    BST_node *node = NULL;
    search_bst_node(tree, key, &node);
    if(!node){
        return data_not_found;
    }
    *dep = node->dep;
    return success;
}

void search_bst_node(BST *tree, int key, BST_node **node){
    if(!tree->root){
        return;
    }

    BST_node *cur = tree->root;
    while(cur){
        if(key == cur->key){
            *node = cur;
            return;
        }
        else if(key < cur->key){
            cur = cur->left;
        }
        else{
            cur = cur->right;
        }
    }
}

void free_bst(BST *tree){
    free_bst_node(tree->root);
    free(tree);
}

void free_bst_node(BST_node *node){
    if(!node){
        return;
    }
    free_bst_node(node->left);
    free_bst_node(node->right);
    free_dep(node->dep);
    free(node);
}