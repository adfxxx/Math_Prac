#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4,
    not_enough_ops = -5,
    div_by_zero = -6,
    neg_power = -7
};

typedef struct Node{
    int data;
    char op;
    struct Node *next;
}Node;

typedef struct{
    struct Node *top;
}Stack;

int count(char *line, int *result);
int convert(char *line, char **result);
int priority(char data);
int check_brackets(char *line);
int read_file(FILE *input, int number, char *input_name);
void remove_n(char *line);
int pop(Stack *stack, int *data, char *op);
int push(Stack *stack, int data, char op);
Stack *create_stack();
void free_stack(Stack *stack);
void print(int state);

int main(int argc, char *argv[]){
    FILE *input;
    int number = 0;
    for(int i = 1; i < argc; i++){
        number++;
        input = fopen(argv[i], "r");
        if(!input){
            print(is_not_open_input);
            continue;
        }
        if(read_file(input, number, argv[i]) == is_not_open_input){
            printf("File with errors is not open, sorry.\n");
            fclose(input);
            return is_not_open_input;
        }
        if(read_file(input, number, argv[i]) == memory_error){
            print(memory_error);
            fclose(input);
            return memory_error;
        }
        fclose(input);
    }
    return success;
}

int count(char *line, int *result){
    Stack *stack = create_stack();
    if(stack == NULL){
        return memory_error;
    }
    int length = strlen(line);
    char cur;
    int num;
    char op;
    int op_1;
    int op_2;
    int res;
    for(int i = 0; i < length; i++){
        cur = line[i];
        if(isdigit(cur) || (cur == '-' && !isdigit(line[i-1]) && isdigit(line[i+1]))){
            if(cur == '-'){
                i++;
                num = line[i]-'0';
                while (i + 1 < length && isdigit(line[i+1])){
                    i++;
                    num = num*10 + (line[i] - '0');
                }
                num *= -1;
                push(stack, num, '\0');
            }
            else{
                num = cur-'0';
                while (i + 1 < length && isdigit(line[i+1])){
                    i++;
                    num = num*10 + (line[i] - '0');
                }
                push(stack, num, '\0');
            }
        }
        else if(cur == ' '){
            continue;
        }
        else if(cur == '+' || cur == '-' || cur == '*' || cur == '/' || cur == '%' || cur == '^'){
            if(stack->top == NULL){
                free_stack(stack);
                return not_enough_ops;
            }
            pop(stack, &op_2, &op);
            if(stack->top == NULL){
                free_stack(stack);
                return not_enough_ops;
            }
            pop(stack, &op_1, &op);
            switch(cur){
                case '+':
                res = op_1 + op_2;
                break;
                case '-':
                res = op_1 - op_2;
                break;
                case '*':
                res = op_1 * op_2;
                break;
                case '/':
                if(op_2 == 0){
                    free_stack(stack);
                    return div_by_zero;
                }
                res = op_1/op_2;
                break;
                case '%':
                if(op_2 == 0){
                    free_stack(stack);
                    return div_by_zero;
                }
                res = op_1%op_2;
                break;
                case '^':
                if(op_2 < 0){
                    free_stack(stack);
                    return neg_power;
                }
                res = pow(op_1, op_2);
                break;
            }
            push(stack, res, '\0');
        }
    }
    if(stack->top == NULL){
        free_stack(stack);
        return fail;
    }
    pop(stack, result, &op);
    if(stack->top != NULL){
        free_stack(stack);
        return fail;
    }
    free_stack(stack);
    return success;
}

int convert(char *line, char **result){
    Stack *stack = create_stack();
    if(stack == NULL){
        return memory_error;
    }
    int length = strlen(line);
    char cur;
    int j = 0;
    int num;
    int data;
    char op;
    for(int i = 0; i < length; i++){
        cur = line[i];
        if(isalpha(cur) || cur == '[' || cur == ']' || cur == '{' || cur == '}'){
            free_stack(stack);
            return wrong_input;
        }
        else if(isdigit(cur) || (cur == '-' && !isdigit(line[i-1]) && isdigit(line[i+1]))){
            if(cur == '-'){
                i++;
                num = line[i]-'0';
                while (i + 1 < length && isdigit(line[i+1])){
                    i++;
                    num = num*10 + (line[i] - '0');
                }
                num *= -1;
                j += sprintf(*result + j, "%d ", num);
            }
            else{
                num = line[i]-'0';
                while (i + 1 < length && isdigit(line[i+1])){
                    i++;
                    num = num*10 + (line[i] - '0');
                }
                j += sprintf(*result + j, "%d ", num);
            }
        }
        else if(cur == ' '){
            continue;
        }
        else if(cur == '('){
            push(stack, 0, cur);
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
        else{
            while(stack->top != NULL && priority(stack->top->op) >= priority(cur)){
                pop(stack, &data, &op);
                (*result)[j] = op;
                j++;
                (*result)[j] = ' ';
                j++;
            }
            push(stack, 0, cur);
        }
    }
    while (stack->top != NULL){
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
    if(data == '^'){
        return 3;
    }
    else if(data == '%' || data == '*' || data == '/'){
        return 2;
    }
    else if(data == '+' || data == '-'){
        return 1;
    }
    else{
        return 0;
    }
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

int read_file(FILE *input, int number, char *input_name){
    if(number > 999){
        return memory_error;
    }
    char *line = NULL;
    size_t len = 0;
    int length = 100;
    char *result = NULL;
    int res;
    char *out_name = (char*)malloc(8*sizeof(char));
    if(out_name == NULL){
        return memory_error;
    }
    sprintf(out_name, "%d.txt", number);
    FILE *output = fopen(out_name, "a");
    if(!output){
        free(result);
        return is_not_open_input;
    }
    int counter = 0;
    while(getline(&line, &len, input) != -1){
        remove_n(line);
        if(check_brackets(line) == fail){
            printf("Error has been written in the file.\n");
            fprintf(output, "Line: %s, number: %d, reason: error in brackets.\n", line, counter);
            counter++;
            continue;
        }
        counter++;
        result = (char*)malloc(len*2*sizeof(char));
        if(result == NULL){
            fclose(output);
            free(line);
            return memory_error;
        }
        if(convert(line, &result) == memory_error){
            fclose(output);
            free(result);
            free(line);
            return memory_error;
        }
        else if(convert(line, &result) == wrong_input){
            printf("Error has been written in the file.\n");
            fprintf(output, "Line: %s, number: %d, reason: wrong symbols in the line.\n", line, counter);
            free(result);
            counter++;
            continue;
        }
        if(count(result, &res) == not_enough_ops){
            printf("Error has been written in the file.\n");
            fprintf(output, "Line: %s, number: %d, reason: not enough ops to finish calculating.\n", line, counter);
            free(result);
            counter++;
            continue;
        }
        else if(count(result, &res) == div_by_zero){
            printf("Error has been written in the file.\n");
            fprintf(output, "Line: %s, number: %d, reason: division by zero.\n", line, counter);
            free(result);
            counter++;
            continue;
        }
        else if(count(result, &res) == fail){
            printf("Error has been written in the file.\n");
            fprintf(output, "Line: %s, number: %d, reason: wrong expression.\n", line, counter);
            free(result);
            counter++;
            continue;
        }
        else if(count(result, &res) == neg_power){
            printf("Error has been written in the file.\n");
            fprintf(output, "Line: %s, number: %d, reason: negative power.\n", line, counter);
            free(result);
            counter++;
            continue;
        }
        else{
            printf("FILE: %s, expression: %s, RPN: %s, calculated result: %d\n", input_name, line, result, res);  
        }
        free(result);
    }
    free(out_name);
    free(line);
    fclose(output);
    return success;
}

void remove_n(char *line){
    int length = strlen(line);
    for(int i = 0; i < length; i++){
        if(line[i] == '\n'){
            line[i] = '\0';
        }
    }
}

void free_stack(Stack *stack){
    while(stack->top != NULL){
        Node *temp = stack->top;
        stack->top = temp->next;
        free(temp);
    }
    free(stack);
}

int pop(Stack *stack, int *data, char *op){
    if(stack->top == NULL){
        return fail;
    }
    Node *temp = stack->top;
    *data = temp->data;
    *op = temp->op;
    stack->top = temp->next;
    free(temp);
    return success;
}

int push(Stack *stack, int data, char op){
    Node *new_node = (Node*)malloc(sizeof(Node));
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
}