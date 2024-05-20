#include "logger.h"
#define LENGTH 20

Status create_logger(Logger **empty_logger, FILE *file){
    if(*empty_logger){
        return wrong_argument;
    }
    *empty_logger = (Logger*)malloc(sizeof(Logger));
    if(!*empty_logger){
        return error_allocation;
    }
    (*empty_logger)->file = file;
    return success;
}

Status new_request_logger(Logger *logger, Data *req, time_t time){
    if(!logger || !logger->file){
        return wrong_argument;
    }
    
    char str_time[LENGTH];
    struct tm *time_tm = localtime(&time);
    sprintf(str_time, "%04d-%02d-%02dT%02d:%02d:%02d", time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: new request in system; request id - %d, department id - %d.\n", str_time, "NEW_REQUEST", req->id, req->dep_id);
    return success;
}

Status department_overloaded_logger(Logger *logger, time_t time, int data_id, int dep_id){
    if(!logger || !logger->file){
        return wrong_argument;
    }
    
    char str_time[LENGTH];
    struct tm *time_tm = localtime(&time);
    sprintf(str_time, "%04d-%02d-%02dT%02d:%02d:%02d", time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: department is overloaded; request id - %d, new department id - %d.\n", str_time, "DEPARTMENT_OVERLOADED", data_id, dep_id);
    return success;
}

Status request_handling_finished_logger(Logger *logger, time_t time, int data_id, int total_time, char *name){
    if(!logger || !logger->file){
        return wrong_argument;
    }
    
    char str_time[LENGTH];
    struct tm *time_tm = localtime(&time);
    sprintf(str_time, "%04d-%02d-%02dT%02d:%02d:%02d", time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: finished handling a request; request id - %d, handling time - %d, operator's name - %s.\n", str_time, "REQUEST_HANDLING_FINISHED", data_id, total_time, name);
    return success;
}

Status request_handling_started_logger(Logger *logger, time_t time, int data_id, char *name){
    if(!logger || !logger->file){
        return wrong_argument;
    }
    
    char str_time[LENGTH];
    struct tm *time_tm = localtime(&time);
    sprintf(str_time, "%04d-%02d-%02dT%02d:%02d:%02d", time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: started handling a request; request id - %d, operator's name - %s.\n", str_time, "REQUEST_HANDLING_STARTED", data_id, name);
    return success;
}

void free_logger(Logger *logger){
    fclose(logger->file);
    free(logger);
}