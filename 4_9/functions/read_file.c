#include "read_file.h"

Status read_input_file(FILE *file, char **heap_type, char **set_type, char **min_time, char **max_time, char **start_time, char **end_time, 
char **overload_coeff, int **deps_id, int **deps_counts, char **count_deps){
    if(!file){
        return wrong_argument;
    }
    
    Status status = get_word_from_file(file, heap_type);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    status = get_word_from_file(file, set_type);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    status = get_word_from_file(file, start_time);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    status = get_word_from_file(file, end_time);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    status = get_word_from_file(file, min_time);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    status = get_word_from_file(file, max_time);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    status = get_word_from_file(file, count_deps);
    if(status != success){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return status;
    }
    
    if(!is_valid_digit(*count_deps)){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return wrong_data;
    }
    
    int deps_count = atoi(*count_deps);
    if(deps_count < 1 || deps_count > 100){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return wrong_data;
    }
    *deps_id = (int*)malloc(sizeof(int)*atoi(*count_deps));
    if(!(*deps_id)){
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return error_allocation;
    }
    *deps_counts = (int*)malloc(sizeof(int)*atoi(*count_deps));
    if(!(*deps_counts)){
        free(*deps_id);
        free_with_amount(7, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps);
        return error_allocation;
    }
    for(int i = 0; i < deps_count; i++){
        char *str = NULL;
        status = get_word_from_file(file, &str);
        char *name = NULL;
        char *op_count = NULL;
        status = get_name_count(&name, &op_count, str);
        if(status != success){
            free_with_amount(9, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps, name, op_count);
            free(*deps_id);
            free(*deps_counts);
        }
        
        if(status != success){
            free_with_amount(9, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps, name, op_count);
            free(*deps_id);
            free(*deps_counts);
            return status;
        }
        if(!is_valid_digit(op_count)){
            return wrong_data;
        }
        int op_c = atoi(op_count);
        int name_i = atoi(name);
        if(name_i < 0){
            free_with_amount(9, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps, name, op_count);
            free(*deps_id);
            free(*deps_counts);
            return wrong_data;
        }
        if(op_c < 10 || op_c > 50){
            free_with_amount(9, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps, name, op_count);
            free(*deps_id);
            free(*deps_counts);
            return wrong_data;
        }
        (*deps_id)[i] = name_i;
        (*deps_counts)[i] = op_c;
        if(status != success){
            free_with_amount(9, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps, name, op_count);
            free(*deps_id);
            free(*deps_counts);
            return status;
        }
    }

    status = get_word_from_file(file, overload_coeff);
    if(status != success){
        free_with_amount(8, *heap_type, *set_type, *start_time, *end_time, *min_time, *max_time, *count_deps, overload_coeff);
        return status;
    }
    return success;
}

Status get_name_count(char **name, char **count, char *str){
    char *first = strtok(str, " ");
    char *second = NULL;
    if(first){
        second = strtok(NULL, " ");
    }
    
    if(first && second){
        *name = (char*)malloc(sizeof(char)*(strlen(first)+1));
        if(!*name){
            return error_allocation;
        }
        *name = first;

        *count = (char*)malloc(sizeof(char)*(strlen(second)+1));
        if(!*count){
            return error_allocation;
        }
        *count = second;
        return success;
    }
    return error;
}

int is_sep(char c){
    if(c == ' ' || c == '\n' || c == '\t'){
        return 1;
    }
    return 0;
}

Status get_word_from_file(FILE *input, char **str){
    size_t len = 0;
    char *line = NULL;
    ssize_t read = getline(&line, &len, input);
    
    if(read == -1) {
        free(line);
        return wrong_file;
    }
    line[strcspn(line, "\n")] = '\0';
    char *start = line;
    *str = (char*)malloc(strlen(line) + 1);
    if (!*str) {
        free(line);
        return error_allocation;
    }

    *str = line;
    (*str)[strlen(line)] = '\0';
    return success;
}

Status get_word_from_line(char *line, char **str, int *index){
    int size = 32;
    *str = (char*)malloc(sizeof(char)*size);
    if(!str){
        return error_allocation;
    }
    
    int len = strlen(line);
    int ind = 0;
    for(int i = *index; i < len; i++){
        if(is_sep(line[i]) || line[i] == EOF){
            *index = i+1;
            break;
        }
        else{
            if(ind >= size - 1){
                char *tmp = (char*)realloc(*str, size*2*sizeof(char));
                if(!tmp){
                    return error_allocation;
                }
                size *= 2;
                *str = tmp;
            }
            (*str)[ind] = line[i];
            ind++;
        }
    }
    (*str)[ind] = '\0';

    return success;
}

Status get_requests(Data ***datas, char *input, int max_prior, int *size, int *capacity){
    FILE *file = fopen(input, "r");
    if(!file){
        return input_file_is_not_open;
    }
    
    char *str;
    char *line = NULL;
    size_t length = 0;
    Status status;
    while(!feof(file) && getline(&line, &length, file)){
        int index = 0;
        status = get_word_from_line(line, &str, &index);
        if(status != success){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        
        if(!is_valid_time(str)){
            free(line);
            free(str);
            fclose(file);
            return wrong_data;
        }
        
        struct tm req_time = {};
        strptime(str, "%Y-%m-%dT%H:%M:%S", &req_time);
        time_t time_req = mktime(&req_time);
        free(str);
         
        status = get_word_from_line(line, &str, &index);
        if(status != success){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        if(!is_valid_digit(str)){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        int prior = atoi(str);
        if(prior > max_prior){
            free(line);
            free(str);
            fclose(file);
            return wrong_data;
        }
        free(str);

        status = get_word_from_line(line, &str, &index);
        if(status != success){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        if(!is_valid_digit(str)){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        int dep_id = atoi(str);
        free(str);

        status = get_word_from_line(line, &str, &index);
        if(status != success){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        if(!is_valid_digit(str)){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        int id = atoi(str);
        free(str);
        
        status = get_word_from_line(line, &str, &index);
        if(status != success){
            free(line);
            free(str);
            fclose(file);
            return status;
        }
        free(line);

        Data *new_data = create_data(time_req, prior, dep_id, id, str);
        if(!new_data){
            free(str);
            fclose(file);
            return error_allocation;
        }
        free(str);

        status = insert_data(datas, new_data, size, capacity);
        if(status != success){
            fclose(file);
            return status;
        }
        length = 0;
    }
    fclose(file);
    return success;
}