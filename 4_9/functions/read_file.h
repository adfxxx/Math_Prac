#ifndef READ_H
#define READ_H
#define _GNU_SOURCE
#include "validation.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

Status read_input_file(FILE *file, char **heap_type, char **set_type, char **min_time, char **max_time, char **start_time, char **end_time, 
char **overload_coeff, int **deps_id, int **deps_counts, char **count_deps);
int is_sep(char c);
Status get_word_from_file(FILE *input, char **str);
Status get_word_from_line(char *line, char **str, int *index);
Status get_name_count(char **name, char **count, char *str);
Status get_requests(Data ***datas, char *input, int max_prior, int *size, int *capacity);

#endif