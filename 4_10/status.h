#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <stdlib.h>

typedef enum Status{
    success = 1,
    error_allocation = -1,
    wrong_argument = -2,
    data_not_found = -3,
    input_file_is_not_open = -4,
    wrong_priority = -5,
    wrong_file = -6,
    wrong_data = -7,
    already_exist = -8,
    wrong_input = -9,
    log_file_is_not_open = -10,
    error = -11
}Status;

void print_error(Status status);

#endif