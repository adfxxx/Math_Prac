#include <stdio.h>
#include <stdlib.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2
};

void print(int state);
int to_base(int number, int base, char **result, int power);

int main(){
    int power = 0;
    printf("Input power: ");
    if(!scanf("%d", &power) || power < 1 || power > 5){
        print(wrong_input);
        return wrong_input;
    }
    fflush(stdin);
    int number = 0;
    printf("Input number: ");
    if(!scanf("%d", &number)){
        print(wrong_input);
        return wrong_input;
    }
    char *result = NULL;
    int total = to_base(number, 1 << power, &result, power);
    if(total == memory_error){
        print(memory_error);
        return memory_error;
    }
    else{
        printf("Result: %s", result);
        free(result);
    }
    return success;
}

int to_base(int number, int base, char **result, int power){
    if(number < 0){
        number = (-1)*number;
    }
    int length = 0;
    if(number == 0){
        length = 2;
        *result = (char*)malloc(length*sizeof(char));
        if (*result == NULL){
            return memory_error;
        }
        (*result)[0] = '0';
        (*result)[1] = '\0';
        return success;
    }
    int temp = number;
    while(temp > 0){
        length++;
        temp /= base;
    }
    *result = (char*)malloc((length+1)*sizeof(char));
    if (*result == NULL){
        return memory_error;
    }
    int bit;
    for(int i = length - 1; i >= 0; i--){
        bit = (number >> (i*power)) & (base - 1);
        if(bit >= 10){
            (*result)[length - 1 - i] = bit + 'A' - 10;
        }
        else{
            (*result)[length - 1 - i] = bit + '0';
        }
    }
    (*result)[length] = '\0';
    return success;
}

void print(int state){
    if(state == wrong_input){
        printf("Wrong input\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
}