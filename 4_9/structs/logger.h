#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "data.h"

typedef struct Logger{
    FILE *file;
}Logger;

Status create_logger(Logger **empty_logger, FILE *file);
Status new_request_logger(Logger *logger, Data *req, time_t time);
Status department_overloaded_logger(Logger *logger, time_t time, int data_id, int dep_id);
Status request_handling_finished_logger(Logger *logger, time_t time, int data_id, int total_time, char *name);
Status request_handling_started_logger(Logger *logger, time_t time, int data_id, char *name);
void free_logger(Logger *logger);

#endif