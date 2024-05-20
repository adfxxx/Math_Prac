#include "data.h"

Data *copy_data(Data *first){
    Data *second = (Data*)malloc(sizeof(Data));
    if(!second){
        return NULL;
    }

    second->getting_time = first->getting_time;
    second->dep_id = first->dep_id;
    second->priority = first->priority;
    second->id = first->id;
    second->text = strdup(first->text);

    return second;
}

int compare_data(Data *first, Data *second){
    if(!first || !second){
        return wrong_argument;
    }

    if(first->priority != second->priority){
        return (first->priority) - (second->priority);
    }

    double diff = difftime(first->getting_time, second->getting_time);
    if(diff < 0){
        return -1;
    }
    else if(diff > 0){
        return 1;
    }
    return 0;
}

Data *create_data(time_t time, int prior, int dep_id, int id, char *text){
    Data *data = (Data*)malloc(sizeof(Data));
    if(!data){
        return NULL;
    }
    data->getting_time = time;
    data->priority = prior;
    data->dep_id = dep_id;
    data->id = id;
    data->text = strdup(text);
    if(!data->text){
        free(data);
        return NULL;
    }
    return data;
}

Status insert_data(Data ***datas, Data *data, int *size, int *capacity){
    if(!*capacity){
        *datas = (Data**)malloc(5*sizeof(Data*));
        if(!*datas){
            return error_allocation;
        }
        *capacity = 5;
    }
    else if(*capacity == *size){
        Data **tmp = (Data**)realloc(*datas, (*capacity)*2*sizeof(Data*));
        if(!tmp){
            free(*datas);
            return error_allocation;
        }
        *capacity *= 2;
        *datas = tmp;
    }

    int index = *size;
    Data *tmp = NULL;
    for(int i = 0; i < *size; i++){
        if((*datas)[i]->getting_time > data->getting_time){
            index = i;
            for(int j = (*size); j > i; j--){
                (*datas)[j] = (*datas)[j-1];
            }
            break;
        }
    }
    (*datas)[index] = data;
    (*size)++;
    return success;
}

int compare_time(time_t time_1, time_t time_2){
    struct tm tm1;
    localtime_r(&time_1, &tm1);
    struct tm tm2;
    localtime_r(&time_2, &tm2);
    if(tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon && tm1.tm_mday == tm2.tm_mday && tm1.tm_hour == tm2.tm_hour && tm1.tm_min == tm2.tm_min){
        return 1;
    }
    return 0;
}

Data *take_req(Data **datas, int *size, time_t time){
    if(!(*size)){
        return NULL;
    }
    if(compare_time(time, datas[0]->getting_time)){
        Data *data = datas[0];
        for(int i = 0; i < *size; i++){
            datas[i] = datas[i+1];
        }
        (*size)--;
        datas[*size] = NULL;
        return data;
    }
    return NULL;
}

void free_data(Data *data){
    if(!data){
        return;
    }
    free(data->text);
    free(data);
}