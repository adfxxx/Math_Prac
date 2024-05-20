#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "functions/validation.h"
#include "functions/read_file.h"
#include "structs/logger.h"

Status create_model(FILE *input, Set **departments, time_t *time_start, time_t *time_end, int *d_c, int **deps_id, int **deps_counts, int *mn_t, int *mx_t);
Status run_model(time_t start_time, time_t end_time, int *size, Data **datas, Logger *logger, Set *deps, int deps_count, int *deps_id, int min_time, int max_time);
Status free_finished_ops(int deps_count, Set *deps, int *deps_id, time_t time, Logger *logger);
Status check_overload(int deps_count, Set *deps, int *deps_id, Logger *logger, time_t time, int last_data_id);
Status load_free_ops(int deps_count, Set *deps, int *deps_id, time_t time, int min_time, int max_time, Logger *logger);

int main(int argc, char *argv[]){
    if(argc < 3){
        print_error(wrong_input);
        return wrong_input;
    }
    if(!is_valid_digit(argv[1])){
        print_error(wrong_priority);
        return wrong_priority;
    }
    int max_priority = atoi(argv[1]);
    FILE *model = fopen(argv[2], "r");
    if(!model){
        print_error(input_file_is_not_open);
        return input_file_is_not_open;
    }

    time_t time_start;
    time_t time_end;
    Set *deps = (Set*)malloc(sizeof(Set));
    if(!deps){
        print_error(error_allocation);
        return error_allocation;
    }
    int deps_count;
    int *deps_id = NULL;
    int *deps_counts = NULL;
    int min_time;
    int max_time;
    Status status = create_model(model, &deps, &time_start, &time_end, &deps_count, &deps_id, &deps_counts, &min_time, &max_time);
    if(status != success){
        fclose(model);
        print_error(status);
        return status;
    }
    fclose(model);
    
    Data **datas = NULL;
    int size = 0;
    int capacity = 0;
    for(int i = 3; i < argc; i++){
        status = get_requests(&datas, argv[i], max_priority, &size, &capacity);
        if(status != success){
            deps->free(deps->dep);
            free(deps);
            free(deps_id);
            free(deps_counts);
            print_error(status);
            return status;
        }
    }
    
    Logger *logger = NULL;
    FILE *log_file = fopen("logs.txt", "w");
    if(!log_file){
        deps->free(deps->dep);
        free(deps);
        free(deps_id);
        free(deps_counts);
        print_error(log_file_is_not_open);
        return log_file_is_not_open;
    }
    status = create_logger(&logger, log_file);
    if(status != success){
        free(deps_id);
        free(deps_counts);
        deps->free(deps->dep);
        free(deps);
        print_error(status);
        return status;
    }
    
    status = run_model(time_start, time_end, &size, datas, logger, deps, deps_count, deps_id, min_time, max_time);
    if(status != success){
        free_logger(logger);
        free(deps_id);
        free(deps_counts);
        free(deps);
        print_error(status);
        return status;
    }

    for(int i = 0; i < size; i++){
        free_data(datas[i]);
    }
    free_logger(logger);
    free(deps_id);
    free(deps_counts);
    free(deps);

    printf("Success.\n");
    return success;
}

Status create_model(FILE *input, Set **departments, time_t *time_start, time_t *time_end, int *d_c, int **deps_id, int **deps_counts, int *mn_t, int *mx_t){
    char *heap_type = NULL;
    char *set_type = NULL;
    char *min_time = NULL;
    char *max_time = NULL;
    char *count_deps = NULL;
    char *overload_coeff = NULL;
    char *start_time = NULL;
    char *end_time = NULL;
    
    Status status = read_input_file(input, &heap_type, &set_type, &min_time, &max_time, &start_time, &end_time, &overload_coeff, deps_id, deps_counts, &count_deps);
    if(status != success){
        return status;
    }

    status = is_valid_data(heap_type, set_type, min_time, max_time, start_time, end_time, overload_coeff, count_deps);
    if(status != success){
        return status;
    }

    struct tm tm_1 = {};
    strptime(start_time, "%Y-%m-%dT%H:%M:%S", &tm_1);
    *time_start = mktime(&tm_1);
    struct tm tm_2 = {};
    strptime(end_time, "%Y-%m-%dT%H:%M:%S", &tm_2);
    *time_end = mktime(&tm_2);
    if(difftime(*time_start, *time_end) > 0){
        free_with_amount(8, heap_type, set_type, min_time, max_time, start_time, end_time, overload_coeff, count_deps);
        return status;
    }
    
    Heap_type h_t;
    get_heap_type(heap_type, &h_t);
    Set_type s_t;
    get_set_type(set_type, &s_t);
    
    int time_min = atoi(min_time);
    int time_max = atoi(max_time);
    *mn_t = time_min;
    *mx_t = time_max;
    double overload_c = atof(overload_coeff);
    int c_d = atoi(count_deps);
    free_with_amount(7, heap_type, set_type, min_time, max_time, overload_coeff, start_time, end_time);
    
    status = create_set(*departments, s_t);
    if(status != success){
        return status;
    }
    
    for(int i = 0; i < c_d; i++){
        int name = ((*deps_id)[i]);
        int op_counts = ((*deps_counts)[i]);

        Department *dep = create_department(name, h_t, op_counts, overload_c);
        if(!dep){
            free_set(*departments);
            return status;
        }
        
        status = (*departments)->insert((*departments)->dep, name, dep);
        if(status != success){
            free_set(*departments);
            return status;
        }
    }
    
    *d_c = c_d;
    return success;
}

Status run_model(time_t start_time, time_t end_time, int *size, Data **datas, Logger *logger, Set *deps, int deps_count, int *deps_id, int min_time, int max_time){
    Status status;
    
    double minute = 60;
    while(start_time < end_time){
        int last_data_id = -1;
        struct tm time;
        localtime_r(&start_time, &time);
        Data *req = take_req(datas, size, start_time);
        while(req){
            last_data_id = req->id;
            status = new_request_logger(logger, req, start_time);
            if(status != success){
                return status;
            }
            status = insert_set(deps, req);
            if(status != success){
                return status;
            }
            req = take_req(datas, size, start_time);
        }
        
        status = check_overload(deps_count, deps, deps_id, logger, start_time, last_data_id);
        if(status != success){
            return status;
        }

        status = free_finished_ops(deps_count, deps, deps_id, start_time, logger);
        if(status != success){
            return status;
        }

        status = load_free_ops(deps_count, deps, deps_id, start_time, min_time, max_time, logger);
        if(status != success){
            return status;
        }

        start_time += minute;
    }
    return success;
}

Status check_overload(int deps_count, Set *deps, int *deps_id, Logger *logger, time_t time, int last_data_id){
    Status status;
    double eps = 0.0000001;
    for(int i = 0; i < deps_count; i++){
        Department *dep = NULL;
        status = deps->find(deps->dep, deps_id[i], &dep);
        if(status != success){
            return status;
        }
        int op_count = dep->busy_ops->count + dep->free_ops->count;
        int data_count = dep->req_count;

        double overload_coeff = data_count/op_count;
        if(overload_coeff < dep->overload_coeff){
            continue;
        }

        Department *min_busy_dep = NULL;
        double min_overload_coeff = -1;
        for(int j = 0; j < deps_count; j++){
            if(deps_id[j] == dep->name){
                continue;
            }
            Department *cur = NULL;
            status = deps->find(deps->dep, deps_id[j], &cur);
            if(status != success){
                return status;
            }
            int cur_op_count = cur->busy_ops->count + cur->free_ops->count;
            int cur_data_count = cur->req_count;
            double cur_overload_coeff = cur_data_count/cur_op_count;
            if(!min_busy_dep || cur_overload_coeff - min_overload_coeff < eps){
                min_busy_dep = cur;
                min_overload_coeff = cur_overload_coeff;
            }
        }
        void *new = NULL;
        status = dep->data->meld(&new, &min_busy_dep->data->dep, &dep->data->dep);
        if(status != success){
            return status;
        }
        min_busy_dep->data->dep = new;
        dep->data = NULL;
        free(dep->data);
        status = create_heap(dep->data, min_busy_dep->data->type);
        if(status != success){
            return status;
        }
        dep->req_count = 0;
        department_overloaded_logger(logger, time, last_data_id, min_busy_dep->name);
    }
    return success;
}

Status free_finished_ops(int deps_count, Set *deps, int *deps_id, time_t time, Logger *logger){
    Status status;
    for(int i = 0; i < deps_count; i++){
        Department *cur = NULL;
        status = deps->find(deps->dep, deps_id[i], &cur);
        if(status != success){
            return status;
        }
        
        status = check_busy_ops(&cur, time, logger);
        if(status != success){
            return status;
        }
    }
    return success;
}

Status load_free_ops(int deps_count, Set *deps, int *deps_id, time_t time, int min_time, int max_time, Logger *logger){
    Status status;
    for(int i = 0; i < deps_count; i++){
        Department *dep = NULL;
        status = deps->find(deps->dep, deps_id[i], &dep);
        if(status != success){
            return status;
        }

        while(dep->free_ops->count && dep->req_count){
            if(!dep->req_count){
                break;
            }
            Data *data = NULL;
            dep->data->delete_max(&(dep->data->dep), &data);
            dep->req_count--;

            status = make_op_busy(&dep, data, time, min_time, max_time, logger);
            if(status != success){
                return status;
            }
        }
    }
    return success;
}