#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4,
    is_not_open_output = -5
};

enum ops{
    Variable,
    Constant,
    And,
    Or,
    Not,
    Impl,
    Coimpl,
    Xor,
    Equiv,
    Sheff,
    Webb
};

typedef struct Node{
    enum ops op;
    char name;
    int constant;
    struct Node *left;
    struct Node *right;
}Node;

int calculate(Node *root, int *values);
int print_table(Node *root, int number, char *names);
char *get_file_name();
void free_tree(Node *root);
Node *create_node(enum ops op, char name, int constant);
Node *get_var(char *line, int *index);
Node *get_expr(char *line, int *index);
int read_file(FILE *input);
int check_brackets(char *line);
int count_vars(char *line, char **names);
int is_op(char *line, int index);
void print(int state);

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
    if(result == wrong_input || result == fail){
        fclose(input);
        print(wrong_input);
        return wrong_input;
    }
    else if(result == is_not_open_output){
        fclose(input);
        print(is_not_open_output);
        return is_not_open_output;
    }
    else if(result == memory_error){
        fclose(input);
        print(memory_error);
        return memory_error;
    }

    return success;
}

void print_tree(Node *root, int level){
    if(root == NULL){
        return;
    }
    print_tree(root->right, level+1);
    for(int i = 0; i < level; i++){
        printf("  ");
    }
    switch (root->op) {
        case Variable:
            printf("Variable: %c\n", root->name);
            break;
        case Constant:
            printf("Constant: %d\n", root->constant);
            break;
        case And:
            printf("AND\n");
            break;
        case Or:
            printf("OR\n");
            break;
        case Not:
            printf("NOT\n");
            break;
        case Impl:
            printf("IMPLICATION\n");
            break;
        case Coimpl:
            printf("COIMPLICATION\n");
            break;
        case Xor:
            printf("XOR\n");
            break;
        case Equiv:
            printf("EQUIVALENCE\n");
            break;
        case Sheff:
            printf("SHEFFER\n");
            break;
        case Webb:
            printf("WEBB\n");
            break;
    }
    print_tree(root->left, level+1);
}

int calculate(Node *root, int *values){
    if(root == NULL){
        return 0;
    }
    int left_result = calculate(root->left, values);
    int right_result = calculate(root->right, values);
    switch(root->op){
        case Variable:
        return values[root->name - 'A'];
        case Constant:
        return root->constant;
        case Not:
        return !left_result;
        case And:
        return left_result && right_result;
        case Or:
        return left_result || right_result;
        case Impl:
        return !left_result || right_result;
        case Coimpl:
        return left_result || !right_result;
        case Equiv:
        return left_result == right_result; 
        case Sheff:
        return !left_result && right_result; 
        case Webb:
        return left_result && right_result;
    }
}

int print_table(Node *root, int number, char *names){
    char *file_name = get_file_name();
    if(file_name == NULL){
        return memory_error;
    }
    FILE *output = fopen(file_name, "w");
    if(!output){
        return is_not_open_output;
    }
    int num_row = 1 << number;
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
        int temp_row = row;
        for (int i = 0; i < number; i++) {
            values[i] = temp_row % 2;
            temp_row /= 2;
            fprintf(output, "%d ", values[i]);
        }
        result = calculate(root, values);
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

void free_tree(Node *root){
    if(root == NULL){
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

Node *create_node(enum ops op, char name, int constant){
    Node *new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        return NULL;
    }
    new_node->op = op;
    new_node->constant = constant;
    new_node->name = name;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

Node *get_var(char *line, int *index){
    Node *node = NULL;
    if(line[*index] == ' '){
        (*index)++;
    }
    if(line[*index] == '('){
        (*index)++;
        node = get_expr(line, index);
        if(node == NULL){
            return NULL;
        }
        if(line[*index] == ')'){
            (*index)++;
        }
        else{
            return NULL;
        }
    }
    else if(isalpha(line[*index]) || line[*index] == '~'){
        if(line[*index] == '~'){
            (*index)++;
            node = create_node(Not, '\0', 0);
            if(node == NULL){
                return NULL;
            }
            node->left = get_var(line, index);
        }
        else{   
            node = create_node(Variable, line[*index], 0);
            if(node == NULL){
                return NULL;
            }
            (*index)++;
        }
    }
    else if(line[*index] == '0' || line[*index] == '1'){
        node = create_node(Constant, '\0', line[*index] - '0');
        if(node == NULL){
            return NULL;
        }
    }
    else if(is_op(line, *index) == fail){
        return NULL;
    }
    return node;
}

Node *get_expr(char *line, int *index){
    Node *node = get_var(line, index);
    if(node == NULL){
        return NULL;
    }
    if(line[*index] == ' '){
        (*index)++;
    }
    int op = is_op(line, *index);
    if(op != fail){
        Node *temp = NULL;
        switch(op){
            case And:
                temp = create_node(And, '\0', 0);
                break;
            case Or:
                temp = create_node(Or, '\0', 0);
                break;
            case Impl:
                temp = create_node(Impl, '\0', 0);
                (*index)++;
                break;
            case Coimpl:
                temp = create_node(Coimpl, '\0', 0);
                (*index)++;
                break;
            case Equiv:
                temp = create_node(Equiv, '\0', 0);
                break;
            case Sheff:
                temp = create_node(Sheff, '\0', 0);
                break;
            case Webb:
                temp = create_node(Webb, '\0', 0);
                break;
        }
        if(temp == NULL){
            return NULL;
        }
        (*index)++;
        temp->left = node;
        temp->right = get_expr(line, index);
        node = temp;
    }
    else{
        return NULL;
    }
    return node;
}

int read_file(FILE *input){
    char *line = NULL;
    size_t len = 0;
    if(getline(&line, &len, input) == -1){
        return wrong_input;
    }
    if(check_brackets(line) == fail){
        free(line);
        return fail;
    }
    int index = 0;
    Node *node = get_expr(line, &index);
    if(node == NULL){
        free(line);
        return fail;
    }
    char *names = (char*)malloc(100*sizeof(char));
    if(names == NULL){
        free(line);
        free_tree(node);
        return memory_error;
    }
    int number = count_vars(line, &names);
    if(number == wrong_input){
        free(names);
        free(line);
        free_tree(node);
        return wrong_input;
    }
    print_tree(node, 0);
    if(print_table(node, number, names) == is_not_open_output){
        free(names);
        free(line);
        free_tree(node);
        return is_not_open_output;
    }
    else if(print_table(node, number, names) == memory_error){
        free_tree(node);
        free(names);
        free(line);
        return memory_error;
    }
    free_tree(node);
    free(names);
    free(line);
    return success;
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

int count_vars(char *line, char **names){
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

int is_op(char *line, int index){
    switch(line[index]){
        case '&':
        return And;
        case '|':
        return Or;
        case '-':
        if(line[index+1] == '>'){
            return Impl;
        }
        else{
            return fail;
        }
        case '+':
        if(line[index+1] == '>'){
            return Coimpl;
        }
        else{
            return fail;
        }
        case '=':
        return Equiv;
        case '!':
        return Sheff;
        case '?':
        return Webb;
        default:
        return fail;
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
}