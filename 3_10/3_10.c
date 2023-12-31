#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    is_not_open_output = -4
};

typedef struct Node{
    char data;
    int count;
    struct Node* next;
    struct Node** children;
}Node;

int check_name(char *name_1, char *name_2);
Node* create_node(char data);
int add_child(Node *parent, Node *child);
Node* build_tree(const char *line);
void print_tree(FILE *output, Node* root, int level);
void delete(Node *root);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 3){
        print(wrong_input);
        return wrong_input;
    }

    int result_check = check_name(argv[1], argv[2]);
    if(result_check == wrong_input){
        print(wrong_input);
        return wrong_input;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }
    FILE *output = fopen(argv[2], "w");
    if(!output){
        print(is_not_open_output);
        fclose(input);
        return is_not_open_output;
    }

    char *line = NULL;
    size_t line_length = 0;
    Node* result;
    while(getline(&line, &line_length, input) != -1){
        result = build_tree(line);
        if(result == NULL){
            print(memory_error);
            free(line);
            delete(result);
            fclose(input);
            fclose(output);
            return memory_error;
        }
        else{
            print_tree(output, result, 0);
            fprintf(output, "\n");
            delete(result);
        }
    }
    printf("Success");
    free(line);
    fclose(input);
    fclose(output);
    return success;
}

int check_name(char *name_1, char *name_2){
    if(!strcmp(name_1, name_2)){
        return wrong_input;
    }
    int length_1 = strlen(name_1);
    int length_2 = strlen(name_2);
    int position_1 = 0;
    int position_2 = 0;
    for (int i = length_1; i > 0 ; --i){
        if (name_1[i] == '\\'){
            position_1 = i + 1;
            break;
        }
    }
    for (int i = length_2; i > 0 ; --i){
        if (name_2[i] == '\\'){
            position_2 = i + 1;
            break;
        }
    }
    int flag = 0;
    if(length_1 == length_2 - position_2 || length_2 == length_1 - position_1){
        if(position_1 == 0){
            int j = 0;
            for(int i = position_2; i < length_2; i++){
                if(name_1[j] == name_2[i]){
                    flag = 1;
                }
                else{
                    flag = 0;
                }
                j++;
            }
        }
        if(position_2 == 0){
            int k = 0;
            for(int i = position_1; i < length_1; i++){
                if(name_1[i] == name_2[k]){
                    flag = 1;
                }
                else{
                    flag = 0;
                }
                k++;
            }
        }
    }
    if(flag == 1){
        return wrong_input;
    }
    else{
        return success;
    }
}

void print_tree(FILE *output, Node *node, int level){
    if(node){
        for(int i = 0; i < level; i++){
            fprintf(output, "  ");
        }
        fprintf(output, "%c\n", node->data);
        for(int i = 0; i < node->count; i++){
            print_tree(output, (node->children)[i], level + 1);
        }
    }
}

Node* build_tree(const char *line){
    int length = strlen(line);
    Node *root = create_node(line[0]);
    Node *cur = root;
    Node *node;
    int result;
    Node **stack = (Node**)malloc(length*sizeof(Node*));
    if(stack == NULL){
        return NULL;
    }
    int stack_top = 0;
    for(int i = 0; i < length; i++){
        if(line[i] == '('){
            node = create_node(line[i+1]);
            if(node == NULL){
                free(stack);
                return NULL;
            }
            stack[stack_top++] = cur;
            result = add_child(cur, node);
            if(result == memory_error){
                free(stack);
                return NULL;
            }
            cur = node;
        }
        else if(line[i] == ','){
            node = create_node(line[i+2]);
            if(node == NULL){
                free(stack);
                return NULL;
            }
            result = add_child(stack[stack_top - 1], node);
            if(result == memory_error){
                free(stack);
                return NULL;
            }
            cur->next = node;
            cur = node;
        }
        else if(line[i] == ')'){
            if(stack_top > 0){
                cur = stack[--stack_top];
            }
        }
    }
    free(stack);
    return root;
}

Node* create_node(char data){
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->children = NULL;
    new_node->count = 0;
    return new_node;
}

int add_child(Node *parent, Node *child){
    (parent->count)++;
    Node **temp = (Node**)realloc(parent->children, (parent->count)*sizeof(Node*));
    if(temp == NULL){
        delete(child);
        return memory_error;
    }
    parent->children = temp;
    (parent->children)[parent->count - 1] = child;
    return success;
}

void delete(Node *root){
    if(!root){
        return;
    }
    for(int i = 0; i < root->count; i++){
        delete((root->children)[i]);
    }
    free(root->children);
    root->children = NULL;
    free(root);
    root = NULL;
}

void print(int state){
    if(state == wrong_input){
        printf("Wrong input\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == is_not_open_input){
        printf("File with data is not open\n");
    }
    else if(state == is_not_open_output){
        printf("Output file is not open\n");
    }
}