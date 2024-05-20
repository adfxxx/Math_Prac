#ifndef DATA_H
#define DATA_H

#include <time.h>
#include "../functions/status.h"
#include <string.h>

typedef struct Data{
    time_t getting_time;
    int priority;
    int dep_id;
    int id;
    char *text;
}Data;

Data *copy_data(Data *first);
int compare_data(Data *first, Data *second);
Data *create_data(time_t time, int prior, int dep_id, int id, char *text);
Status insert_data(Data ***datas, Data *data, int *size, int *capacity);
int compare_time(time_t time_1, time_t time_2);
Data *take_req(Data **datas, int *size, time_t time);
void free_data(Data *data);

#endif