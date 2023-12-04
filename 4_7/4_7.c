#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4,
    wrong_operation = -5
};

typedef struct{
    char *name;
    int number;
}MemoryCell;

void free_array(MemoryCell *memory, int count);
int print_elem(MemoryCell *memory, int count, char *name);
void print_all_memory(MemoryCell *memory, int count);
int change_op(char op, int num_1, int num_2, MemoryCell **memory, char *word, int *memory_size, int *count);
int get_str_result(int number, char **result);
int get_vars(char *line, int i, MemoryCell **memory, int *memory_size, int *count, char *value_1, char op, char *word, int minus);
MemoryCell *find_var(MemoryCell *memory, int memory_size, const char *name);
int save(char *name, char *value, int *memory_size, MemoryCell **memory, int *count, int minus);
int compare_names(const void *a, const void *b);
int read_file(FILE *input, MemoryCell **memory, int *memory_size, int *count);
int check_digit(const char *value);
int check_name(const char *name);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 2){
        print(wrong_input);
        return wrong_input;
    }
    
    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }
    MemoryCell *memory = NULL;
    int memory_size = 1;
    int count = 0;
    int result = read_file(input, &memory, &memory_size, &count);
    fclose(input);
    free_array(memory, count);
    if(result == memory_error){
        print(memory_error);
        return memory_error;
    }
    else if(result == wrong_operation){
        print(wrong_operation);
        return wrong_operation;
    }
    printf("Success!");
    return success;
}

void free_array(MemoryCell *memory, int count){
    for (int i = 0; i < count; i++){
        free(memory[i].name);
    }
}

int get_str_result(int number, char **result){
    int i = 0;
    int flag = 0;
    if(number < 0){
        flag = 1;
        number *= (-1);

    }
    while(number != 0){
        (*result)[i] = (number % 10) + '0';
        number /= 10;
        i++;
        if(i == 98){
            return wrong_input;
        }
    }
    if(flag == 1){
        (*result)[i] = '-';
        i++;
    }
    (*result)[i] = '\0';
    int length = strlen(*result);
    i = 0;
    int j = length - 1;
    char symbol;
    while(i < j){
        symbol = (*result)[i];
        (*result)[i] = (*result)[j];
        (*result)[j] = symbol;
        i++;
        j--;
    }
    return success;
}

int print_elem(MemoryCell *memory, int count, char *name){
    if(find_var(memory, count, name) == NULL){
        return fail;
    }
    for(int i = 0; i < count; i++){
        if(!strcmp(name, memory[i].name)){
            printf("%s = %d\n", memory[i].name, memory[i].number);
        }
    }
}

void print_all_memory(MemoryCell *memory, int count){
    printf("Elements: ");
    for(int i = 0; i < count; i++){
        printf("%s = %d", memory[i].name, memory[i].number);
        if(i < count - 1){
            printf(", ");
        }
    }
    printf(".\n");
}   

int change_op(char op, int num_1, int num_2, MemoryCell **memory, char *word, int *memory_size, int *count){
    int result = 0;
    switch(op){
        case '+':
            result = num_1 + num_2;
            break;
        case '-':
            result = num_1 - num_2;
            break;
        case '*':
            result = num_1 * num_2;
            break;
        case '/':
            if(num_2 == 0){
                return wrong_input;
            }
            result = num_1 / num_2;
            break;
        case '%':
            if(num_2 == 0){
                return wrong_input;
            }
            result = num_1 % num_2;
            break;
    }
    char *res = (char*)malloc(100*sizeof(char));
    if(res == NULL){
        return memory_error;
    }
    if(result / 10 == 0){
        res[0] = result + '0';
        res[1] = '\0';
    }
    else{
        if(get_str_result(result, &res) == wrong_input){
            free(res);
            return wrong_input;
        }
    }
    if(save(word, res, memory_size, memory, count, 0) == memory_error){
        free(res);
        return memory_error;
    }
    free(res);
    return success;
}

int get_vars(char *line, int i, MemoryCell **memory, int *memory_size, int *count, char *value_1, char op, char *word, int minus){
    int num_1;
    MemoryCell *var_1;
    if(check_digit(value_1) == fail){
        num_1 = atoi(value_1);
        if(minus == 1){
            num_1 *= (-1);
        }
    }
    else if(check_digit(value_1) == wrong_input){
        return wrong_input;
    }
    else{
        var_1 = find_var(*memory, *count, value_1);
        if(var_1 == NULL){
            return wrong_operation;
        }
        num_1 = var_1->number;
    }
    int size = 100;
    char *value_2 = (char*)malloc(size*sizeof(char));
    if(value_2 == NULL){
        return memory_error;
    }
    int j = 0;
    int num_2;
    MemoryCell *var_2;
    while(line[i] != ';'){
        value_2[j] = line[i];
        i++;
        j++;
        if(j == 100){
            size *= 2;
            char *temp = (char*)realloc(value_2, size*sizeof(char));
            if(temp == NULL){
                free(value_2);
                return memory_error;
            }
            value_2 = temp;
        }
    }
    value_2[j] = '\0'; 
    if(check_digit(value_2) == fail){
        num_2 = atoi(value_2);
    }
    else if(check_digit(value_2) == wrong_input){
        free(value_2);
        return wrong_input;
    }
    else{
        var_2 = find_var(*memory, *count, value_2);
        if(var_2 == NULL){
            return wrong_operation;
        }
        num_2 = var_2->number;
    }
    int result = change_op(op, num_1, num_2, memory, word, memory_size, count);
    free(value_2);
    if(result == wrong_input){
        return wrong_input;
    }
    else if(result == memory_error){
        return memory_error;
    }
    else{
        return success;
    }
}

MemoryCell *find_var(MemoryCell *memory, int count, const char *name){
    int left = 0;
    int right = count - 1;
    int i;
    int check;
    while(left <= right){
        i = (left+right)/2;
        check = strcmp(memory[i].name, name);
        if(check == 0){
            return &memory[i];
        }
        else if(check > 0){
            right = i - 1;
        }
        else{
            left = i + 1;
        }
    }
    return NULL;
}

int save(char *name, char *value, int *memory_size, MemoryCell **memory, int *count, int minus){
    int num;
    int index;
    MemoryCell *var;
    if(check_digit(value) == fail){
        num = atoi(value);
        if(minus == 1){
            num *= (-1);
        }
    }
    else if(check_digit(value) == wrong_input){
        return wrong_input;
    }
    else{
        var = find_var(*memory, *count, value);
        if(var == NULL){
            return wrong_operation;
        }
        num = var->number;
    }
    var = find_var(*memory, *count, name);
    if(var != NULL){
        var->number = num;
    }
    else{
        (*count)++;
        if((*count) + 1 >= (*memory_size)*2){
            (*memory_size) *= 2;
            MemoryCell *temp = (MemoryCell*)realloc(*memory, (*memory_size)*sizeof(MemoryCell));
            if(temp == NULL){
                (*memory_size) /=2;
                return memory_error;
            }
            *memory = temp;
        }
        (*memory)[*count-1].name = strdup(name);
        if((*memory)[*count-1].name == NULL){
            return memory_error;
        }
        (*memory)[*count-1].number = num;
        qsort(*memory, *count, sizeof(MemoryCell), compare_names);
    }
    return success;
}

int compare_names(void const *a, const void *b){
    return strcmp(((MemoryCell const*)a)->name, ((MemoryCell const*)b)->name);
}

int read_file(FILE *input, MemoryCell **memory, int *memory_size, int *count){
    char *line = NULL;
    size_t len = 0;
    int size = 100;
    char *word = NULL;
    int i = 0;
    int j = 0;
    char *value_1 = NULL;
    int result;
    int flag = 0;
    char *temp_pr = NULL;
    int minus = 0;
    while(getline(&line, &len, input) != -1){
        minus = 0;
        size = 100;
        i = 0;
        j = 0;
        word = (char*)malloc(size*sizeof(char));
        if(word == NULL){
            free(line);
            return memory_error;
        }
        value_1 = (char*)malloc(size*sizeof(char));
        if(value_1 == NULL){
            free(word);
            free(line);
            return memory_error;
        }
        while(line[i] != '='){
            if(line[i] == '='){
                break;
            }
            word[j] = line[i];
            i++;
            j++;
            if(i == 5){
                temp_pr = strdup(word);
                if(temp_pr == NULL){
                    free(line);
                    free(word);
                    return memory_error;
                }
                temp_pr[j] = '\0';
                if(!strcmp(temp_pr, "print")){
                    if(line[i] == ';'){
                        if(*count == 0){
                            flag = 0;
                            size = 100;
                            i = 0;
                            j = 0;
                            free(word);
                            free(value_1);
                            continue;
                        }
                        print_all_memory(*memory, *count);
                        flag = 1;
                        break;
                    }
                    else{
                        i++;
                        j = 0;
                        while(line[i] != ';'){
                            value_1[j] = line[i];
                            j++;
                            i++;
                        }
                        value_1[j] = '\0';
                        if(print_elem(*memory, *count, value_1) == fail){
                            free(line);
                            free(value_1);
                            free(word);
                            return wrong_operation;
                        }
                        flag = 1;
                        break;
                    }
                    free(temp_pr);
                }
            }
            if(i == 100){
                size *= 2;
                char *temp = (char*)realloc(word, size*sizeof(char));
                if(temp == NULL){
                    free(line);
                    free(value_1);
                    free(word);
                    return memory_error;
                }
            }
        }
        if(flag == 1){
            flag = 0;
            size = 100;
            i = 0;
            j = 0;
            free(value_1);
            free(word);
            continue;
        }
        i++;
        word[j] = '\0';
        if(check_name(word) == fail){
            free(word);
            flag = 0;
            size = 100;
            i = 0;
            j = 0;
            free(value_1);
            continue;
        }
        j = 0;
        size = 100;
        if(line[i] == '-'){
            minus = 1;
            i++;
        }
        while(line[i] != ';' && line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/' && line[i] != '%'){
            value_1[j] = line[i];
            i++;
            j++;
            if(j == 100){
                size *= 2;
                char *temp = (char*)realloc(value_1, size*sizeof(char));
                if(temp == NULL){
                    free(line);
                    free(word);
                    free(value_1);
                    return memory_error;
                }
            }
            if(line[i] == ';' || line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%'){
                break;
            }
        }
        value_1[j] = '\0';
        char op;
        if(line[i] == ';'){
            result = save(word, value_1, memory_size, memory, count, minus);
            if(result == memory_error){
                free(line);
                free(word);
                free(value_1);
                return memory_error;
            }
        }  
        else if(line[i] != ';'){
            op = line[i];
            i++;
            result = get_vars(line, i, memory, memory_size, count, value_1, op, word, minus);
            if(result == wrong_input){
                continue;
            }
            else if(result == memory_error){
                free(line);
                free(word);
                free(value_1);
                return memory_error;
            }
            else if(result == wrong_operation){
                free(line);
                free(word);
                free(value_1);
                return wrong_operation;
            }
        }
        free(word);
        free(value_1);   
    }
    free(line);
    free(word);
    free(value_1);
    return success;
}

int check_digit(const char *value){
    int length = strlen(value);
    int check_1 = 0;
    int check_2 = 0;
    for(int i = 0; i < length; i++){
        if(isalpha(value[i])){
            check_1++;
        }
        else if(isdigit(value[i]) || value[i] == '-'){
            check_2++;
        }
        else{
            return wrong_input;
        }
    }
    if(check_1 != 0 && check_2 == 0){
        return success;
    }
    else if(check_1 == 0 && check_2 != 0){
        return fail;
    }
    else if(check_1 != 0 && check_2 != 0){
        return wrong_input;
    }
}

int check_name(const char *name){
    int length = strlen(name);
    for(int i = 0; i < length; i++){
        if(!isalpha(name[i])){
            return fail;
        }
    }
    return success;
}

void print(int state){
    if(state == wrong_input){
        printf("\nWrong input. Try again.\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == is_not_open_input){
        printf("File with data is not open\n");
    }
    else if(state == wrong_operation){
        printf("Wrong line in the file. Impossible finish calculations.\n");
    }
}