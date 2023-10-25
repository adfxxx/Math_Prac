#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

enum errors{
    success = 1,
    wrong_input = -1,
    wrong_flag = -2,
    memory_error = -3
};

int check_flag(char *flag);
int get_length(char *line);
void reverse(char *line, int length, char **result);
void get_up(char *line, int length, char **result);
int flag_n(char *line, int length, char **result);
char *flag_c(unsigned int seed, char **strings, int size);
void str_cpy(char *str, char *result);

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Wrnog amount");
        return wrong_input;
    }
    if(check_flag(argv[1]) == wrong_flag){
        printf("Wrong flag");
        return wrong_flag;
    }
    char flag = argv[1][1];
    char *line = argv[2];
    int length = get_length(line);
    unsigned int seed = 0;
    switch(flag){
        case 'l':
            if(argc != 3){
                printf("Wrong amount");
                return wrong_input;
            }
            printf("Length of the line: %d", length);
            return 0;
        case 'r':
            if(argc != 3){
                printf("Wrong amount");
                return wrong_input;
            }
            char *reversed = (char*)malloc((length + 1)*sizeof(char));
            if (reversed == NULL){
                printf("Memory error");
                return memory_error;
            }
            reverse(line, length, &reversed);
            printf("Reversed string: %s", reversed);
            free(reversed);
            return 0;
        case 'u':
            if(argc != 3){
                printf("Wrong amount");
                return wrong_input;
            }
            char *upper = (char*)malloc(length*sizeof(char));
            if (upper == NULL){
                printf("Memory error");
                return memory_error;
            }
            upper[length] = '\0';
            get_up(line, length, &upper);
            printf("New line: %s", upper);
            free(upper);
            return 0;
        case 'n':
            if(argc != 3){
                printf("Wrong amount");
                return wrong_input;
            }
            char *change_line = (char*)malloc(length*sizeof(char));
            if (change_line == NULL){
                printf("Memory error");
                return memory_error;
            }
            if(flag_n(line, length, &change_line) == success){
                change_line[length] = '\0';
                printf("New line: %s", change_line);
                free(change_line);
            }
            else{ 
                printf("Error");
                free(change_line);
            }
            return 0;
        case 'c':
            seed = atoi(argv[3]);
            if(seed <= 0){
                printf("Wrong input");
                return wrong_input;
            }
            int size = argc - 3;
            char **strings = (char **)malloc(size*sizeof(char*));
            if(strings == NULL){
                printf("Memory error");
                return memory_error;
            }
            int k = 0;
            for (int i = 2; i < argc; i++){
                if (i == 3){
                    continue;
                }
                else{
                    strings[k] = argv[i];
                    k++;
                }
            }
            if(!strings){
                printf("Wrong input");
                return wrong_input;
            }
            char *result = flag_c(seed, strings, size);
            printf("Concatenated string: %s", result);
            for(int i = 0; i < argc; i++){
                free(strings[i]);
            }
            free(strings);
            free(result);
            return 0;
    }
    
}

int check_flag(char *flag){
    if(flag[0] == '-'){
        if (flag[1] == 'l' || flag[1] == 'r' || flag[1] == 'u' || flag[1] == 'n' || flag[1] == 'c'){
            return success;
        }
    }
    return wrong_flag;
}

int get_length(char *line){
    int i = 0;
    while (line[i] != '\0'){
        i++;
    }
    return i;
}

void reverse(char *line, int length, char **result){
    for (int i = 0; i < length; i++){
        (*result)[i] = line[length - 1 - i];
    }
    (*result)[length] = '\0';
}

void get_up(char *line, int length, char **result){
    for (int i = 0; i < length; i++){
        if((line[i] >= 'a' && line[i] <= 'z') && i % 2 == 0){
            (*result)[i] = (line[i] - 32);
        }
        else{
            (*result)[i] = line[i];
        }
    }
}

int flag_n(char *line, int length, char **result){
    char *digits = (char*)malloc(sizeof(char)*length);
    if(digits == NULL){
        return memory_error;
    }
    int count_d = 0;
    char *letters = (char*)malloc(sizeof(char)*length);
    if(letters == NULL){
        return memory_error;
    }
    int count_l = 0;
    char *symbols = (char*)malloc(sizeof(char)*length);
    if(symbols == NULL){
        return memory_error;
    }
    int count_s = 0;
    for (int i = 0; i < length; i++){
        if(isdigit(line[i])){
            digits[count_d] = line[i];
            count_d++;
        }
        else if(isalpha(line[i])){
            letters[count_l] = line[i];
            count_l++;
        }
        else{
            symbols[count_s] = line[i];
            count_s++;
        }
    }
    for (int i = 0; i < count_d; i++){
        (*result)[i] = digits[i];
    }
    for (int i = 0; i < count_l; i++){
        (*result)[count_d + i] = letters[i];
    }
    for (int i = 0; i < count_s; i++){
        (*result)[count_d + count_l + i] = symbols[i];
    }
    free(digits);
    free(letters);
    free(symbols);
    return success;
}

void str_cpy(char *str, char *result){
    int i = 0;
    while (str[i] != '\0'){
        result[i] = str[i];
        i++;
    }
    result[i] = '\0';
}

char *flag_c(unsigned int seed, char **strings, int size){
    srand(seed);
    char *temp;
    int length = 0;
    for (int i = 0; i < size - 1; i++){
        int j = rand() % (size - i) + i;
        temp = strings[i];
        strings[i] = strings[j];
        strings[j] = temp;
    }
    for (int i = 0; i < size; i++){
        length += get_length(strings[i]);
    }
    char *result = (char*)malloc(sizeof(char)*(length+1));
    if(result == NULL){
        return NULL;
    }
    int index = 0;
    for (int i = 0; i < size; i++){
        length = get_length(strings[i]);
        for (int j = 0; j < length; j++){
            result[index] = strings[i][j];
            index++;
        }
    }
    return result;
}