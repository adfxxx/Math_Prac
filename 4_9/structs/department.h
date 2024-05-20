#ifndef DEP_H
#define DEP_H

#include "../heaps/heap.h"
#include "operator.h"
#include "logger.h"

typedef struct Department{
    int name;
    Heap *data;
    int req_count;
    Operators *free_ops;
    Operators *busy_ops;
    double overload_coeff;
}Department;

Department *create_department(int name, Heap_type type, int ops_count, double overload_coeff);
Status insert_req(Department *dep, Data *data);
Status check_busy_ops(Department **dep, time_t time, Logger *logger);
Status make_op_busy(Department **dep, Data *data, time_t start_time, int min_time, int max_time, Logger *logger);
void free_dep(Department *dep);

#endif