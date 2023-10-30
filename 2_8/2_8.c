#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2
};

char *total_sum(int base, int amount, ...);
char *sum_in_st(char *num_1, char *num_2, int base);
int transform(char *number);
char *remove_zeros(char **result, int length);
int check_number(char *number, int base);
void print(int state);

int main(){
    int base = 10;
    char *result = total_sum(base, 2, "000", "0");
    if (result != NULL){
        printf("Your sum is %s", result);
    }
    return success;
}

char *total_sum(int base, int amount, ...){
    va_list numbers;
    va_start(numbers, amount);
    char *result = va_arg(numbers, char*);
    for(int i = 1; i < amount; i++){
        char *num = va_arg(numbers, char*);
        if(check_number(num, base) == wrong_input){
            print(wrong_input);
            return NULL;
        }
        result = sum_in_st(result, num, base);
        if(result == NULL){
            print(memory_error);
            return NULL;
        }
    }
    va_end(numbers);
    return result;
}

char *sum_in_st(char *num_1, char *num_2, int base){
    int length_1 = strlen(num_1);
    int length_2 = strlen(num_2);
    if (remove_zeros(&num_1, length_1) == NULL || remove_zeros(&num_2, length_2) == NULL) {
        return NULL;
    }
    length_1 = strlen(num_1);
    length_2 = strlen(num_2);
    int max_length = (length_1 >= length_2) ? length_1 + 1 : length_2 + 1;
    char *result = (char*)calloc(max_length, sizeof(char));
    if(result == NULL){
        return NULL;
    }
    int sum = 0;
    int ost = 0;
    int index = max_length;
    int digit_1 = 0;
    int digit_2 = 0;
    
    for(int i = length_1 - 1, j = length_2 - 1; i >= 0 || j >= 0 || ost > 0; i--, j--){
        digit_1 = transform(&num_1[i]);
        digit_2 = transform(&num_2[j]);
        if(j < 0){
            digit_2 = 0;
        }
        if (i < 0){
            digit_1 = 0;
        }
        sum = digit_1 + digit_2 + ost;
        ost = sum / base;
        result[--index] = (sum % base) + '0';
    }
    char *real_result = (char*)malloc(max_length - index + 1); 
    if(real_result == NULL){
        free(result);
        return NULL;
    }
    strcpy(real_result, result + index);
    free(result);
    if(!strcmp(real_result, "0")){
        return real_result;
    }
    if(remove_zeros(&real_result, max_length - index + 1) == NULL){
        free(result);
        free(real_result);
        return NULL;
    }
    return real_result;
}

char *remove_zeros(char **result, int length){
    int count = 0;
    while((*result)[count] == '0'){
        count++;
    }
    if(count == length){ 
        char *new_result = (char *)malloc(2 * sizeof(char));
        if(new_result == NULL){
            return NULL;
        }
        new_result[0] = '0';
        new_result[1] = '\0';
        free(*result);
        *result = new_result;
        return *result;
    }
    else if(count > 0){
        char *new_result = (char *)malloc((length - count + 1) * sizeof(char));
        if(new_result == NULL){
            return NULL;
        }
        strcpy(new_result, *result + count);
        free(*result);
        *result = new_result;
        return *result;
    }
    else {
        return *result;
    }
}
int transform(char *number){
    if(isdigit(*number)){
        return *number - '0';
    }
    else{
        return *number - 'A' + 10;
    }
}

int check_number(char *number, int base){
    int num;
    while(*number != '\0'){
        if(isdigit(*number)){
            num = *number - '0';
            if(num >= base){
                return wrong_input;
            }
        }
        else if(isalpha(*number)){
            num = *number - 'A' + 10;
            if(num >= base){
                return wrong_input;
            }
        }
        else{
            return wrong_input;
        }
        number++;
    }
    return success;
}

void print(int state){
    if(state == wrong_input){
        printf("Your input is wrong\n");
    }
    if(state == memory_error){
        printf("Memory error\n");
    }
}