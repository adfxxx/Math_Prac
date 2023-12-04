#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4,
    is_not_open_output = -5
};

typedef struct Stack_node{
    char data;
    int op;
    struct Stack_node *next;
}Stack_node;

typedef struct{
    struct Stack_node *top;
}Stack;

typedef struct Node{
    char data;
    struct Node *left;
    struct Node *right;
}Node;

typedef struct Node_stack_n{
    Node *tree_node;
    struct Node_stack_n *next;
}Node_stack_n;

typedef struct Node_stack{
    Node_stack_n *top;
}Node_stack;

void free_tree(Node *root);
int calculate(Node *root, int *values, int number, char *names);
int print_table(Node *root, int number, char *names);
char *get_file_name();
Node *build_tree(char *line);
Node *create_node(char data);
Node *pop_node(Node_stack *stack);
int push_node(Node_stack *stack, Node *node);
Node_stack *create_node_stack();
void free_node_stack(Node_stack *stack);
int convert(char *line, char **result);
int priority(char data);
int read_file(FILE *input);
Stack *create_stack();
int push(Stack *stack, int data, char op);
void pop(Stack *stack, int *data, char *op);
void free_stack(Stack *stack);
int get_vars(char *line, char **names);
int check_brackets(char *line);
void remove_n(char *line);
void print(int state);
void print_spaces(int count);
void print_tree(Node *root, int depth);

int main(int argc, char *argv[]){
    if(argc != 2){
        print(wrong_input);
        return wrong_input;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }
    int result = read_file(input);
    fclose(input);
    if(result == wrong_input){
        print(wrong_input);
        return wrong_input;
    }
    else if(result == is_not_open_output){
        print(is_not_open_output);
        return is_not_open_output;
    }
    else if(result == memory_error){
        print(memory_error);
        return memory_error;
    }
    else if(result == fail){
        print(fail);
        return fail;
    }

    printf("Success.\n");
    return success;
}

void free_tree(Node *root){
    if(root == NULL){
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int calculate(Node *root, int *values, int number, char *names){
    if(root == NULL){
        return fail;
    }
    int left = calculate(root->left, values, number, names);
    int right = calculate(root->right, values, number, names);
    if(root->data == '1'){
        return root->data - '0';
    }
    else if(root->data == '0'){
        return root->data  - '0';
    }
    else if(isalpha(root->data)){
        for(int i = 0; i < number; i++){
            if(root->data == names[i]){
                return values[i];
            }
        }
    }
    else{
        switch(root->data){
            case '~':
            return !left;
            case '&':
            return left && right;
            case '|':
            return left || right;
            case '-':
            return !left || right;
            case '+':
            return left || !right;
            case '<':
            return (left || right) && !(left && right);
            case '=':
            return left == right; 
            case '!':
            return !(left && right); 
            case '?':
            return !(left || right);
        }
    }
}

int print_table(Node *root, int number, char *names){
    if(root == NULL){
        return fail;
    }
    char *file_name = get_file_name();
    if(file_name == NULL){
        return memory_error;
    }
    FILE *output = fopen(file_name, "w");
    if(!output){
        return is_not_open_output;
    }
    int num_row = 1 << number; //2^number
    for(int i = 0; i < number; i++){
        fprintf(output, "%c ", names[i]);
    }
    fprintf(output, "| Result\n");
    for(int i = 0; i < number; i++){
        fprintf(output, "--");
    }
    fprintf(output, "|--------\n");
    int *values = (int*)malloc(number*sizeof(int));
    if(values == NULL){
        fclose(output);
        return memory_error;
    }
    int result;
    for (int row = 0; row < num_row; row++) {
        for (int i = 0; i < number; i++) {
            values[i] = (row>>i) & 1; //row/2^i
            fprintf(output, "%d ", (row>>i) & 1);
        }
        int result = calculate(root, values, number, names);
        fprintf(output, "| %d\n", result);
    }
    free(values);
    fclose(output);
    return success;
}

char *get_file_name(){
    srand(time(NULL));
    char *file_name = (char*)malloc(10*sizeof(char));
    if(file_name == NULL){
        return NULL;
    }
    int symbol;
    for(int i = 0; i < 6; i++){
        symbol = rand() % 36;
        if(symbol < 26){
            file_name[i] = 'A' + symbol;
        }
        else{
            file_name[i] = '0' + (symbol - 26);
        }
    }
    file_name[6] = '.';
    file_name[7] = 't';
    file_name[8] = 'x';
    file_name[9] = 't';
    file_name[10] = '\0';
    return file_name;
 }

 Node *build_tree(char *line){
    Node_stack *stack = create_node_stack();
    if(stack == NULL){
        return NULL;
    }
    int length = strlen(line);
    char cur;
    for (int i = 0; i < length; i++){
        cur = line[i];
        if(isalpha(cur) || cur == '1' || cur == '0'){
            Node *new_node = create_node(cur);
            if(new_node == NULL){
                free_node_stack(stack);
                return NULL;
            }
            if(push_node(stack, new_node) == memory_error){
                free_node_stack(stack);
                return NULL;
            }
        }
        else if(cur == ' '){
            continue;
        }
        else if(cur == '~'){
            Node *var = pop_node(stack);
            Node *op_node = create_node(cur);
            if(op_node == NULL){
                free_node_stack(stack);
                return NULL;
            }
            op_node->left = var;
            op_node->right = NULL;
            if(push_node(stack, op_node) == memory_error){
                free_node_stack(stack);
                return NULL;
            }
        }
        else{
            Node *right = pop_node(stack);
            Node *left = pop_node(stack);
            Node *op_node = create_node(cur);
            if(op_node == NULL){
                free_node_stack(stack);
                return NULL;
            }
            op_node->left = left;
            op_node->right = right;
            if(push_node(stack, op_node) == memory_error){
                free_node_stack(stack);
                return NULL;
            }
        }
    }
    Node *root = pop_node(stack);
    free_node_stack(stack);
    return root;
}

Node *create_node(char data){
    Node *new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        return NULL;
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

Node *pop_node(Node_stack *stack){
    if(stack->top == NULL){
        return NULL;
    }
    Node_stack_n *temp = stack->top;
    Node *node = temp->tree_node;
    stack->top = temp->next;
    free(temp);
    return node;
}

int push_node(Node_stack *stack, Node *node){
    Node_stack_n *new_node = (Node_stack_n*)malloc(sizeof(Node_stack_n));
    if(new_node == NULL){
        return memory_error;
    }
    new_node->tree_node = node;
    new_node->next = stack->top;
    stack->top = new_node;
    return success;
}

Node_stack *create_node_stack(){
    Node_stack *new_node = (Node_stack*)malloc(sizeof(Node_stack));
    if(new_node == NULL){
        return NULL;
    }
    new_node->top = NULL;
    return new_node;
}

void free_node_stack(Node_stack *stack){
    while(stack->top != NULL){
        Node_stack_n *temp = stack->top;
        stack->top = temp->next;
        free(temp);
    }
    free(stack);
}

int convert(char *line, char **result){
    Stack *stack = create_stack();
    if(stack == NULL){
        return memory_error;
    }
    int length = strlen(line);
    char cur;
    int j = 0;
    char name;
    int data;
    char op;
    for(int i = 0; i < length; i++){
        cur = line[i];
        if(cur == '>'){
            continue;
        }
        if(isalpha(cur)){
            name = toupper(line[i]);
            (*result)[j] = name;
            j++;
            (*result)[j] = ' ';
            j++;
        }
        else if(isdigit(cur) && cur != '1' && cur != '0'){
            free_stack(stack);
            return wrong_input;
        }
        else if(cur == '1' || cur == '0'){
            (*result)[j] = cur;
            j++;
            (*result)[j] = ' ';
            j++;
        }
        else if(cur == ' '){
            continue;
        }
        else if(cur == '('){
            if(push(stack, 0, cur) == memory_error){
                free_stack(stack);
                return memory_error;
            }
        }
        else if(cur == ')'){
            while(stack->top != NULL && stack->top->op != '('){
                pop(stack, &data, &op);
                (*result)[j] = op;
                j++;
                (*result)[j] = ' ';
                j++;
            }
            pop(stack, &data, &op);
        }
        else if(cur == '&' || cur == '|' || cur == '~' || cur == '-' || cur == '+' || cur == '<' || cur == '=' || cur == '!' || cur == '?'){
            if((cur == '-' || cur == '+' || cur == '<') && line[i+1] != '>'){
                free_stack(stack);
                return wrong_input;
            }
            while(stack->top != NULL && priority(stack->top->op) >= priority(cur)){
                pop(stack, &data, &op);
                (*result)[j] = op;
                j++;
                (*result)[j] = ' ';
                j++;
            }
            if(push(stack, 0, cur) == memory_error){
                free_stack(stack);
                return memory_error;
            }
        }
        else{
            free_stack(stack);
            return wrong_input;
        }
    }
    while(stack->top != NULL){
        pop(stack, &data, &op);
        (*result)[j] = op;
        j++;
        (*result)[j] = ' ';
        j++;
    }
    (*result)[j] = '\0';
    free_stack(stack);
    return success;
}

int priority(char data){
    if(data == '~'){
        return 3;
    }
    else if(data == '?' || data == '!' || data == '+' || data == '&'){
        return 2;
    }
    else if(data == '|' || data == '-' || data == '<' || data == '='){
        return 1;
    }
    else{
        return 0;
    }
}

int read_file(FILE *input){
    char *line = NULL;
    size_t len = 0;
    if(getline(&line, &len, input) == -1){
        return wrong_input;
    }
    remove_n(line);
    if(check_brackets(line) == fail){
        free(line);
        return wrong_input;
    }
    int index = 0;
    int length = strlen(line);
    char *result = (char*)malloc(length*2*sizeof(char));
    if(result == NULL){
        free(line);
        return memory_error;
    }
    char *names = (char*)malloc(100*sizeof(char));
    if(names == NULL){
        free(result);
        free(line);
        return memory_error;
    }
    int number = get_vars(line, &names);
    if(number == wrong_input){
        free(result);
        free(line);
        free(names);
        return wrong_input;
    }
    int res = convert(line, &result);
    if(res == memory_error){
        free(result);
        free(line);
        free(names);
        return memory_error;
    }
    else if(res == wrong_input){
        free(result);
        free(line);
        free(names);
        return wrong_input;
    }
    free(line);
    Node *root = build_tree(result);
    if(root == NULL){
        free(result);
        free(names);
        return memory_error;
    }
    free(result);
    res = print_table(root, number, names);
    free_tree(root);
    free(names);
    if(res == fail){
        return fail;
    }
    else if(res == memory_error){
        return memory_error;
    }
    else if(res == is_not_open_output){
        return is_not_open_output;
    }
    return success;
}

void free_stack(Stack *stack){
    while(stack->top != NULL){
        Stack_node *temp = stack->top;
        stack->top = temp->next;
        free(temp);
    }
    free(stack);
}

void pop(Stack *stack, int *data, char *op){
    if(stack->top == NULL){
        return;
    }
    Stack_node *temp = stack->top;
    *data = temp->data;
    *op = temp->op;
    stack->top = temp->next;
    free(temp);
    return;
}

int push(Stack *stack, int data, char op){
    Stack_node *new_node = (Stack_node*)malloc(sizeof(Stack_node));
    if(new_node == NULL){
        return memory_error;
    }
    new_node->data = data;
    new_node->op = op;
    new_node->next = stack->top;
    stack->top = new_node;
    return success;
}

Stack *create_stack(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if(stack == NULL){
        return NULL;
    }
    stack->top = NULL;
    return stack;
}

int get_vars(char *line, char **names){
    int number = 0;
    int vars[26] = {0};
    while(*line){
        if(isalpha(*line) && !vars[toupper(*line) - 'A']){
            if(number == 100){
                return wrong_input;
            }
            vars[toupper(*line) - 'A'] = 1;
            (*names)[number] = (char)toupper(*line);
            number++; 
        }
        line++;
    }
    (*names)[number] = '\0';
    return number;
}

int check_brackets(char *line){
    int length = strlen(line);
    int check = 0;
    for(int i = 0; i < length; i++){
        if(line[i] == '('){
            check++;
        }
        else if(line[i] == ')'){
            check--;
        }
    }
    if(check != 0){
        return fail;
    }
    else{
        return success;
    }
}

void remove_n(char *line){
    int length = strlen(line);
    for(int i = 0; i < length; i++){
        if(line[i] == '\n'){
            line[i] = '\0';
        }
    }
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
    else if(state == fail){
        printf("Error with tree, it might be empty.\n");
    }
}

// void print_spaces(int count) {
//     for (int i = 0; i < count; i++) {
//         printf(" ");
//     }
// }

// void print_tree(Node *root, int depth) {
//     if (root == NULL) {
//         return;
//     }
//     print_tree(root->right, depth + 1);
//     print_spaces(depth * 4);
//     if (isalpha(root->data)) {
//         printf("%c\n", root->data);
//     } else {
//         printf("%c\n", root->data);
//     }
//     print_tree(root->left, depth + 1);
// }