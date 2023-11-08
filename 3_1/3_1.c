#include <stdio.h>
#include <stdlib.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2
};

void print(int state);
int to_base(int number, int base, char **result, int power);
int sum(int a, int b);
int sub(int a, int b);

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
    else if(total == memory_error){
        print(wrong_input);
        return wrong_input;
    }
    else{
        printf("Result: %s", result);
        free(result);
    }
    return success;
}

int to_base(int number, int base, char **result, int power){
    if(power < 1 || power > 5){
        return wrong_input;
    }
    if(number < 0){
        number = sum(~number, 1);
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
        length = sum(length, 1);
        temp >>= power;
    }
    *result = (char*)malloc((sum(length,1)*sizeof(char)));
    if (*result == NULL){
        return memory_error;
    }
    (*result)[length] = '\0';
    int bit;
    int index;
    for(int i = length - 1; i >= 0; i--){
        bit = number & sub(base, 1);
        if(bit >= 10){
            temp = sum(bit, 'A');
            (*result)[i] = sub(temp, 10);
        }
        else{
            (*result)[i] = sum(bit, '0');
        }
        number >>= power;
    }
    return success;
}

int sum(int a, int b){
    int temp;
    while(b != 0){
        temp = a & b;
        a ^= b;
        b = temp << 1;
    }
    return a;
}

int sub(int a, int b){
    int temp;
    while(b != 0){
        temp = (~a) & b;
        a ^= b;
        b = temp << 1;
    }
    return a;
}

void print(int state){
    if(state == wrong_input){
        printf("Wrong input\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
}