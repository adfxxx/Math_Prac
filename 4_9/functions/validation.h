#ifndef VAL_H
#define VAL_H

#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include "../sets/set.h"

int is_valid_digit(char *str);
int is_valid_double(char *str);
int is_valid_time(char *str);
int is_leap_year(int year);
Status is_valid_data(char *heap_type, char *set_type, char *min_time, char *max_time, char *start_time, char *end_time, char *overload_coeff, char *count_deps);
Status get_heap_type(char *str, Heap_type *type);
Status get_set_type(char *str, Set_type *type);
void free_with_amount(int amount,...);

#endif