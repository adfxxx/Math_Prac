#include "department.h"

Department *create_department(int name, Heap_type type, int ops_count, double overload_coeff){
    Department *dep = (Department*)malloc(sizeof(Department));
    if(!dep){
        return NULL;
    }

    dep->name = name;
    Heap *tmp = (Heap*)malloc(sizeof(Heap));
    if(!tmp){
        free(dep);
        return NULL;
    }
    Status status = create_heap(tmp, type);
    if(status != success){
        free(dep);
        free(tmp);
        return NULL;
    }
    dep->data = tmp;
    dep->req_count = 0;
    dep->overload_coeff = overload_coeff;
    dep->busy_ops = create_ops();
    if(!dep->busy_ops){
        free(dep);
        return NULL;
    }
    dep->free_ops = create_ops();
    if(!dep->free_ops){
        free(dep);
        free(dep->busy_ops);
        return NULL;
    }

    for(int i = 0; i < ops_count; i++){
        Operator *op = create_op();
        if(!op){
            free(dep);
            free(dep->busy_ops);
            free(dep->free_ops);
            return NULL;
        }
        insert_op(dep->free_ops, op);
    }
    return dep;
}

Status insert_req(Department *dep, Data *data){
    if(!dep || !data){
        return wrong_argument;
    }
    dep->req_count++;
    return dep->data->insert(&dep->data->dep, data);
}

Status check_busy_ops(Department **dep, time_t time, Logger *logger){
    if(!*dep){
        return wrong_argument;
    }
    if(!(*dep)->busy_ops->count){
        return success;
    }

    Operator *cur = (*dep)->busy_ops->op;
    Operator *op = NULL;
    Operator *prev = NULL;
    while(cur){
        if(compare_time(time, cur->time->end)){
            op = cur;
            if(!prev){
                (*dep)->busy_ops->op = cur->next;
            }
            else{
                prev->next = cur->next;
            }

            (*dep)->busy_ops->count--;
            op->next = NULL;
            insert_op((*dep)->free_ops, op);
            request_handling_finished_logger(logger, time, op->data->id, op->time->total_time, op->name);
            free_data(op->data);
            free(op->time);
            op->data = NULL;
            op->time = NULL;
        }
        prev = cur;
        cur = cur->next;
    }
    return success;
}

Status make_op_busy(Department **dep, Data *data, time_t start_time, int min_time, int max_time, Logger *logger){
    if((*dep && !(*dep)->free_ops->count) || (!dep) || !data){
        return wrong_argument;
    }

    Operator *op = get_op((*dep)->free_ops);
    op->data = data;
    op->time = create_op_time(start_time, min_time, max_time);
    if(!op->time){
        return error_allocation;
    }
    insert_op((*dep)->busy_ops, op);

    request_handling_started_logger(logger, start_time, data->id, op->name);
    return success;
}

void free_dep(Department *dep){
    if(!dep){
        return;
    }
    free_ops(dep->busy_ops);
    free_ops(dep->free_ops);
    free_heap(dep->data);
    free(dep);
}