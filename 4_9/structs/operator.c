#include "operator.h"

Operators *create_ops(){
    Operators *ops = (Operators*)malloc(sizeof(Operators));
    if(!ops){
        return NULL;
    }
    ops->op = NULL;
    ops->count = 0;
    return ops;
}

Operator *create_op(){
    Operator *op = (Operator*)malloc(sizeof(Operator));
    if(!op){
        return NULL;
    }
    op->time = NULL;
    op->data = NULL;
    op->name = generate_random_name();
    op->next = NULL;
    return op;
}

void insert_op (Operators *arr, Operator *op){
    op->next = NULL;
    Operator *cur = arr->op;
    if(!cur){
        arr->op = op;
        arr->count++;
        return;
    }
    while(cur->next){
        cur = cur->next;
    }
    cur->next = op;
    arr->count++;
}

char *generate_random_name(){
    int max = 15;
    int min = 1;
    int length = rand() % (max - min + 1) + min;
    char *new_name = (char*)malloc((length + 1)*sizeof(char));
    if(!new_name){
        return NULL;
    }
    new_name[length] = '\0';
    for(int i = 0; i < length;){
        new_name[i] = rand() % ('Z' - '0' + 1) + '0';
        if(isalpha(new_name[i]) || isdigit(new_name[i])){
            i++;
        }
    }
    return new_name;
}

Operator *get_op(Operators *ops){
    Operator *op = ops->op;
    ops->op = op->next;
    ops->count--;
    return op;
}

Operator_time *create_op_time(time_t start_time, int min_time, int max_time){
    Operator_time *time = (Operator_time*)malloc(sizeof(Operator_time));
    if(!time){
        return NULL;
    }

    int total_time = rand() % (max_time - min_time + 1) + min_time;
    time_t end_time = start_time +(total_time*60);
    struct tm *end = localtime(&end_time);
    time->start = start_time;
    time->end = end_time;
    time->total_time = total_time;
    return time;

}

void free_ops(Operators *ops){
    if(!ops){
        return;
    }
    Operator *cur = ops->op;
    Operator *next = NULL;
    while(cur){
        next = cur->next;
        free_op(cur);
        cur = next;
    }
    free(ops);
    ops = NULL;
}

void free_op(Operator *op){
    free(op->name);
    op->name = NULL;
    free_data(op->data);
    if(op->time){
        free(op->time);
    }
    op->time = NULL;
    op->next = NULL;
    free(op);
}