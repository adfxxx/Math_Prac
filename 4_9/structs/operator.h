#ifndef OP_H
#define OP_H

#include "data.h"
#include <ctype.h>
#include <string.h>

typedef struct Operator_time{
    time_t start;
    time_t end;
    int total_time;
}Operator_time;

typedef struct Operator{
    char *name;
    Data *data;
    Operator_time *time;
    struct Operator *next;
}Operator;


typedef struct Operators{
    Operator *op;
    int count;
}Operators;

Operators *create_ops();
Operator *create_op();
char *generate_random_name();
void insert_op (Operators *arr, Operator *op);
Operator *get_op(Operators *ops);
Operator_time *create_op_time(time_t start_time, int min_time, int max_time);
void free_ops(Operators *ops);
void free_op(Operator *op);

#endif