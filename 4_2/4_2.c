#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4
};

enum commands{
    load = 2,
    save = 3,
    Rand = 4,
    concat = 5,
    Free = 6,
    Remove = 7,
    copy = 8,
    sort = 9,
    shuffle = 10,
    stats = 11,
    Print = 12
};

typedef struct{
    char name;
    int *data;
    int size;
}Array;

void free_array(Array *arrays, int count);
int get_max(Array *array);
int get_min(Array *array);
int get_freq(Array *array);
double get_average(Array *array);
double get_max_dev(Array *array, double average);
void stats_cmd(Array *array);
int comparison_random(const void *arg_1, const void *arg_2);
int comparison_minus(const void *arg_1, const void *arg_2);
int comparison_plus(const void *arg_1, const void *arg_2);
void sort_cmd(Array *array, char symbol);
int copy_cmd(Array *array, Array *array_2, int index_1, int index_2);
int remove_cmd(Array *array, int index, int number);
void free_cmd(Array *array);
int concat_cmd(Array *array, Array *array_2);
int rand_cmd(char name, int count, int lb, int rb, Array *array);
int save_cmd(Array *array, char *file);
Array *find_array(char name, Array *array, int count);
void print_array(Array *array, int start, int end);
int load_cmd(Array *array, char *file);
Array *create(char name);
int change_command(int command, char *line, int *count, Array *arrays, char *file_name);
int get_name(char *line, int option, char **name, int i);
int read_file(FILE *input, Array *arrays, int *count, char *file_name);
int check_str(char *line, int flag, int i);
int command_check(const char *command);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 2){
        return wrong_input;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }
    
    int count = 0;
    Array arrays[26];
    int result = read_file(input, arrays, &count, argv[1]);
    if(result == memory_error){
        fclose(input);
        return memory_error;
    }

    fclose(input);
    free_array(arrays, count);
    return success;
}

void free_array(Array *arrays, int count){
    for (int i = 0; i < count; i++){
        free_cmd(&arrays[i]);
    }
}

void print_array(Array *array, int start, int end){
    printf("Array %c: [", array->name);
    if(end - start == 1){
        printf("%d]\n", array->data[start]);
        return;
    }
    for (int i = start; i < end; ++i) {
        printf("%d", array->data[i]);
        if (i < end - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int get_max(Array *array){
    int max = array->data[0];
    for(int i = 0; i < array->size; i++){
        if(array->data[i] > max){
            max = array->data[i];
        }
    }
    return max;
}

int get_min(Array *array){
    int min = array->data[0];
    for(int i = 0; i < array->size; i++){
        if(array->data[i] < min){
            min = array->data[i];
        }
    }
    return min;
}

int get_freq(Array *array){
    int max_count = 0;
    int max_elem = array->data[0];
    int cur;
    int cur_count = 0;
    for(int i = 0; i < array->size; i++){
        cur = (int)array->data[i];
        cur_count = 1;
        for(int j = i+1; j < array->size; j++){
            if((int)array->data[j] == cur){
                cur_count++;
            }
        }
        if (cur_count > max_count || (cur_count == max_count && cur > max_elem)){
            max_count = cur_count;
            max_elem = cur;
        }
    }
    return max_elem;
}

double get_average(Array *array){
    double sum = 0.0;
    for(int i = 0; i < array->size; i++){
        sum += array->data[i];
    }
    return sum/array->size;
}

double get_max_dev(Array *array, double average){
    double max_dev = 0.0;
    double dev;

    for(int i = 0; i < array->size; i++){
        dev = fabs(array->data[i] - average);
        if(dev > max_dev){
            max_dev = dev;
        }
    }
    return max_dev;
}

void stats_cmd(Array *array){
    printf("Information about %c: size - %d,", array->name, array->size);
    if(array->size == 0){
        printf(" max elem - none, min elem - none, the most frequent - none, average - none, , max deviation - none.\n");

    }
    else{
        int max = get_max(array);
        int min = get_min(array);
        int freq = get_freq(array);
        double average = get_average(array);
        double max_dev = get_max_dev(array, average);
        printf(" max elem - %d, min elem - %d, the most frequent - %d, average - %.2f, max deviation - %.2f.\n", max, min, freq, average, max_dev);
    }
}

int comparison_random(const void *arg_1, const void *arg_2){
    return rand() % 3 - 1;
}

int comparison_minus(const void *arg_1, const void *arg_2){
    const int *number_1 = (const int*)arg_1;
    const int *number_2 = (const int*)arg_2;
    return -(*number_1 - *number_2);
}

int comparison_plus(const void *arg_1, const void *arg_2){
    const int *number_1 = (const int*)arg_1;
    const int *number_2 = (const int*)arg_2;
    return *number_1 - *number_2;
}

void sort_cmd(Array *array, char symbol){
    if(symbol == '+'){
        qsort(array->data, array->size, sizeof(int), comparison_plus);
    }
    if (symbol == '-'){
        qsort(array->data, array->size, sizeof(int), comparison_minus);
    }
}

int copy_cmd(Array *array, Array *array_2, int index_1, int index_2){
    int old_size = array_2->size;
    array_2->size = (index_2 - index_1 + 1) + array_2->size;
    int *temp = (int*)realloc(array_2->data, array_2->size*sizeof(int));
    if(temp == NULL){
        return memory_error;
    }
    array_2->data = temp;
    for(int i = index_1; i <= index_2; i++){
        array_2->data[old_size] = array->data[i];
        old_size++;
    }
    return success;
}

int remove_cmd(Array *array, int index, int number){
    for(int i = index; i < array->size - number; i++){
        array->data[i] = array->data[i+number];
    }

    array->size -= number;
    int *temp = (int*)realloc(array->data, array->size*sizeof(int));
    if(temp == NULL){
        return memory_error;
    }
    array->data = temp;
}

void free_cmd(Array *array){
    if(array->data == NULL){
        return;
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
}

int concat_cmd(Array *array, Array *array_2){
    int old_size = array->size;
    array->size = array->size + array_2->size;
    int *temp = (int*)realloc(array->data, array->size*sizeof(int));
    if(temp == NULL){
        return memory_error;
    }
    array->data = temp;
    int j = 0;
    for(int i = old_size; i < array->size; i++){
        array->data[i] = array_2->data[j];
        j++;
    }
    return success;
}

int rand_cmd(char name, int count, int lb, int rb, Array *array){
    srand(time(NULL));
    array->size = count;
    array->data = (int*)malloc(count*sizeof(int));
    if(array->data == NULL){
        return memory_error;
    }

    for(int i = 0; i < count; i++){
        array->data[i] = rand() % (rb - lb + 1) + lb;
    }

    return success;
}

int save_cmd(Array *array, char *file){
    FILE *output = fopen(file, "w");
    if(!output){
        return is_not_open_input;
    }
    fprintf(output, "Array %c: [", array->name);
    for(int i = 0; i < array->size; i++){
        fprintf(output, "%d", array->data[i]);
        if(i < array->size - 1){
            fprintf(output, ", ");
        }
    }
    fprintf(output, "]\n");
    fclose(output);
    return success;
}

Array *find_array(char name, Array *arrays, int count){
    for(int i = 0; i < count; i++){
        if(arrays[i].name == name){
            return &arrays[i];
        }
    }
    return NULL;
}

int load_cmd(Array *array, char *file){
    FILE *input = fopen(file, "r");
    if(!input){
        return is_not_open_input;
    }
    int size = 100;
    array->data = (int*)malloc(size*sizeof(int));
    if(array->data == NULL){
        fclose(input);
        return memory_error;
    }
    int value;
    int index = 0;
    int *temp;
    int result;
    while((result = fscanf(input, "%d", &value)) != EOF){
        if(result != 1){
            fscanf(input, "%*[^0-9]");
            continue;
        }
        if(index == size){
            size*=2;
            temp = realloc(array->data, size*sizeof(int));
            if(temp == NULL){
                fclose(input);
                free(array->data);
                array->data = NULL;
                return memory_error;
            }
            array->data = temp;
        }
        array->data[index] = value;
        index++;
    }
    array->size = index;
    fclose(input);
    return success;
}

int change_command(int command, char *line, int *count, Array *arrays, char *file_name){
    int i = 0;
    int k = 0;
    char name;
    char *name_str = (char*)malloc(2*sizeof(char));
    if(name_str == NULL){
        return memory_error;
    }
    Array *array;
    int length = 100;
    char *file = NULL;
    char *temp;
    char *number_str = NULL;
    int number;
    int index;
    int lb;
    int rb;
    char name_2;
    Array *array_2;
    int flag = 0;
    switch(command){
        case load:
        file = (char*)malloc(length*sizeof(char));
        if(file == NULL){
            return memory_error;
        }
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        array = create(name);
        if(array == NULL){
            return memory_error;
        }
        arrays[*count] = *array;
        (*count)++;
        i = get_name(line, 2, &file, i);
        if(i == fail) break;
        else if(i == memory_error) return memory_error;
        if(!strcmp(file, file_name)){
            printf("Wrong file name.\n");
            free(file);
            break;
        }
        if(load_cmd(&arrays[*count-1], file) == memory_error){
            free(file);
            return memory_error;
        }
        print_array(&arrays[*count-1], 0, arrays[*count-1].size);
        free(file);
        break;

        case save:
        file = (char*)malloc(length*sizeof(char));
        if(file == NULL){
            return memory_error;
        }
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        i = get_name(line, 2, &file, i);
        if(i == fail) break;
        else if(i == memory_error) return memory_error;
        if(!strcmp(file, file_name)){
            printf("Wrong file name.\n");
            free(file);
            break;
        }
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not in the system.\n");
            free(file);
            break;
        }
        if(save_cmd(array, file) == is_not_open_input){
            printf("File is not open.\n");
            free(file);
            break;
        }
        printf("Successful writing.\n");
        free(file);
        break;

        case Rand:
        number_str = (char*)malloc(4*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 1, &name_str, i);
        name = name_str[0];
        i = get_name(line, 3, &number_str, i);
        if(i == fail) printf("Too many elements.\n");
        number = atoi(number_str);
        free(number_str);
        number_str = NULL;
        
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error){
            free(number_str);
            return memory_error;
        } 
        lb = atoi(number_str);
        free(number_str);
        number_str = NULL;
        number_str  = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error){
            free(number_str);
            return memory_error;
        } 
        rb = atoi(number_str);                      
        free(number_str);
        number_str = NULL;
        array = create(name);
        if(array == NULL){
            return memory_error;
        }
        arrays[*count] = *array;
        (*count)++;
        if(rand_cmd(name, number, lb, rb, &arrays[*count-1]) == memory_error){
            print(memory_error);
            break;
        }
        print_array(&arrays[*count-1], 0, arrays[*count-1].size);
        break;

        case concat:
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        get_name(line, 1, &name_str, i);
        name_2 = name_str[0];
        array = find_array(name, arrays, *count);
        array_2 = find_array(name_2, arrays, *count);
        if(array == NULL || array_2 == NULL){
            printf("There are no arrays with these name in the system.\n");
            break;
        }
        if(concat_cmd(array, array_2) == memory_error){
            return memory_error;
        }
        print_array(array, 0, array->size);
        break;

        case Free:
        while(line[i] != '('){
            i++;
        }
        name = tolower(line[i+1]);
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("There is no array with this name in the system.\n");
            break;
        }
        free_cmd(array);
        print_array(array, 0, array->size);
        break;

        case Remove:
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error) return memory_error;
        index = atoi(number_str);
        free(number_str);
        number_str = NULL;
        if(index > array->size - 1 || index < 0){
            printf("Wrong index.\n");
            break;
        }
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error) return memory_error;
        number = atoi(number_str);
        free(number_str);
        number_str = NULL;
        if(number > array->size || number > array->size - index || number < 0){
            printf("Wrong amount of elements to delete.\n");
            break;
        }
        if(remove_cmd(array, index, number) == memory_error){
            return memory_error;
        }
        print_array(array, 0, array->size);
        break;

        case copy:
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error) return memory_error;
        index = atoi(number_str);
        free(number_str);
        number_str = NULL;
        if(index > array->size - 1 || index < 0){
            printf("Wrong first index.\n");
            break;
        }
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error) return memory_error;
        number = atoi(number_str);
        free(number_str);
        number_str = NULL;
        if(number > array->size - 1 || number < 0 || number < index){
            printf("Wrong second index.\n");
            break;
        }
        get_name(line, 1, &name_str, i);
        name_2 = name_str[0];
        array_2 = find_array(name_2, arrays, *count);
        if(array_2 == NULL){
            array_2 = create(name_2);
            if(array_2 == NULL){
                return(memory_error);
            }
            arrays[*count-1] = *array_2;
            (*count)++;
        }
        if(copy_cmd(array, array_2, index, number) == memory_error){
            return memory_error;
        }
        print_array(array_2, 0, array_2->size);
        break;

        case sort:
        while(line[i] != '+' && line[i] != '-' && line[i] != ';'){
            i++;
        }
        if(line[i] == ';'){
            print(wrong_input);
            break;
        }
        name = tolower(line[i-1]);
        if(!isalpha(name)){
            break;
        }
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        name_2 = line[i];
        if(name_2 != '+' && name_2 != '-'){
            print(wrong_input);
            break;
        }
        sort_cmd(array, name_2);
        print_array(array, 0, array->size);
        break;

        case shuffle:
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        qsort(array->data, array->size, sizeof(int), comparison_random);
        print_array(array, 0, array->size);
        break;

        case stats:
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        stats_cmd(array);
        break;

        case Print:
        i = get_name(line, 1, &name_str, i);
        if(i == fail) break;
        name = name_str[0];
        array = find_array(name, arrays, *count);
        if(array == NULL){
            printf("Array is not found in the system.\n");
            break;
        }
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        while(i != ',' && i != ';'){
            if(line[i] == ' '){
                i++;
            }
            else if(line[i] == ',' || line[i] == ';'){
                break;
            }
            else{
                if(!isdigit(line[i])){
                    print_array(array, 0, array->size);
                    flag = 1;
                    break;
                }
                number_str[k] = line[i];
                k++;
                i++;
                if(k == length){
                    length*=2;
                    temp = (char*)realloc(number_str, length*sizeof(char));
                    if(temp == NULL){
                        free(number_str);
                        break;
                    }
                    number_str = temp;
                }
            }
        }
        if(flag == 1){
            free(number_str);
            number_str = NULL;
            break;
        }
        number_str[k] = '\0';
        number = atoi(number_str);
        free(number_str);
        number_str = NULL;
        
        if(line[i] == ';'){
            if(number < 0 || number > array->size-1){
                printf("Wrong index");
                break;
            }
            print_array(array, number, number+1);
            break;
        }
        i++;
        k = 0;
        length = 100;
        number_str = (char*)malloc(length*sizeof(char));
        if(number_str == NULL){
            return memory_error;
        }
        i = get_name(line, 4, &number_str, i);
        if(i == memory_error) return memory_error;
        index = atoi(number_str);
        free(number_str);
        number_str = NULL;
        if(index > array->size - 1 || index < number){
            printf("Wrong second index.\n");
            break;
        }
        print_array(array, number, index+1);
        break;
    }
    free(name_str);
    name_str = NULL;
    return success;
}

int get_name(char *line, int option, char **name, int i){
    int length = 100;
    int k = 0;
    char *temp;
    switch(option){
        case 1:
        while(line[i] != ',' && line[i] != ';'){
            i++;
        }
        (*name)[k] = tolower(line[i-1]);
        (*name)[k+1] = '\0';
        if(!isalpha((*name)[0])){
            return fail;
        }
        return i+2;

        case 2:
        while(line[i] != ';' && line[i] != EOF){
            if(k == length){
                length*=2;
                temp = (char*)realloc(*name, length*sizeof(char));
                if(temp == NULL){
                    free((*name));
                    return memory_error;
                }
                (*name) = temp;
            }
            (*name)[k] = line[i];
            k++;
            i++;
        }
        (*name)[k] = '\0';
        break;

        case 3:
        while(i != ','){
            if(line[i] == ','){
                break;
            }
            (*name)[k] = line[i];
            k++;
            i++;
            if(k == 3 && line[i] != ','){
                return fail;
            }
        }
        (*name)[k] = '\0';
        return i+2;

        case 4:
        while(i != ',' && i != ';'){
            if(line[i] == ' '){
                i++;
            }
            else if(line[i] == ','){
                break;
            }
            else{
                (*name)[k] = line[i];
                k++;
                i++;
                if(k == length){
                    length*=2;
                    temp = (char*)realloc(*name, length*sizeof(char));
                    if(temp == NULL){
                        return memory_error;
                    }
                    (*name) = temp;
                }
            }
        }
        (*name)[k] = '\0';
        return i+2;
    }
    return success;
}

Array *create(char name){
    Array *new_array = (Array*)malloc(sizeof(Array));
    if(new_array == NULL){
        return NULL;
    }
    new_array->name = name;
    new_array->data = NULL;
    new_array->size = 0;

    return new_array;
}

int check_str(char *line, int flag, int i){
    int length = strlen(line);
    int check = 0;
    if(flag == Free){
        for(i; i < length; i++){
            if(line[i] == '('){
                check++;
            }
            else if(line[i] == ';'){
                check++;
            }
            else if(line[i] == ')'){
                check++;
            }
            else if(line[i] == ' '){
                check++;
            }
            else if(line[i] == ','){
                check++;
            }
        }
        if(check == 3){
            return success;
        }
        else{
            return fail;
        }
    }
    for(i; i < length; i++){
        if(line[i] == ','){
            check++;
        }
        else if(line[i] == ';'){
            check++;
        }
        else if(line[i] == ' '){
            check++;
        }
    }
    if(flag == load || flag == save || flag == concat){
        if(check == 4){
            return success;
        }
        else{
            return fail;
        }
    }
    if(flag == Rand || flag == copy){
        if(check == 8){
            return success;
        }
        else{
            return fail;
        }
    }
    if(flag == Remove){
        if(check == 6){
            return success;
        }
        else{
            return fail;
        }
    }
    if(flag == sort || flag == shuffle || flag == stats){
        if(check == 2){
            return success;
        }
        else{
            return fail;
        }
    }
    if(flag == Print){
        if(check == 4 || check == 6){
            return success;
        }
        else{
            return fail;
        }
    }
}

int read_file(FILE *input, Array *arrays, int *count, char *file_name){
    char *line = NULL;
    int len = 0;
    int length = 8;
    char *command_str = NULL;
    int i = 0;
    int k = 0;
    int command;
    int result;
    while(getline(&line, &len, input) != -1){
        command_str = (char*)malloc(length*sizeof(char));
        if(command_str == NULL){
            print(memory_error);
        }
        while(line[i] != ' ' && line[i] != '(' && line[i] != ';' && line[i] != '\n' && line[i] != EOF){
            if(i >= 7){
                free(command_str);
                command_str = NULL;
                break;
            }
            command_str[k] = line[i];
            k++;
            i++;
        }
        if(command_str != NULL){
            command_str[k] = '\0';
            command = command_check(command_str);
            if(command == fail){
                print(wrong_input);
            }
            else{
                result = check_str(line, command, i);
                if(result == success){
                    if(change_command(command, line, count, arrays, file_name) == memory_error){
                        print(memory_error);
                    }
                }
                else{
                    print(wrong_input);
                }
            }
        }
        else{
            print(wrong_input);
        }
        free(command_str);
        command_str = NULL;
        i = 0;
        k = 0;
    }
    free(line);
    return success;
}

int command_check(const char *command){
    if(!strcmp(command, "Load")){
        return load;
    }
    if(!strcmp(command, "Save")){
        return save;
    }
    if(!strcmp(command, "Rand")){
        return Rand;
    }
    if(!strcmp(command, "Concat")){
        return concat;
    }
    if(!strcmp(command, "Free")){
        return Free;
    }
    if(!strcmp(command, "Remove")){
        return Remove;
    }
    if(!strcmp(command, "Copy")){
        return copy;
    }
    if(!strcmp(command, "Sort")){
        return sort;
    }
    if(!strcmp(command, "Shuffle")){
        return shuffle;
    }
    if(!strcmp(command, "Stats")){
        return stats;
    }
    if(!strcmp(command, "Print")){
        return Print;
    }
    return fail;
}

void print(int state){
    if(state == wrong_input){
        printf("Wrong input\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == is_not_open_input){
        printf("File with data is not open\n");
    }
}