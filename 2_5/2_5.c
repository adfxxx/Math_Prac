#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

enum errors{
    success = 1,
    wrong_input = -1,
    fail = -2,
    wrong_flag = -3,
    memory_error = -4,
    print_error = -5
};

int overfprintf(FILE *stream, const char *format, ...);
int oversprintf(char *buf, const char *format, ...);
int check_flag(char first, char second);
void change_flags(FILE *stream, va_list *args, char first, char second);
int flag_ro(int number, char **result);
int flag_zr(unsigned int number, char **result);
int get_fib(int number, int *count);
int flag_cv(int number, int base, char **result, int flag);
int flag_to(char *number, int base, int flag);
int flag_m(const void *ptr, size_t size, unsigned char **result);
void print(int state);


int main(){
    overfprintf(stdout, "Roman: %Ro\nZeckendorf: %Zr\nTo base (low): %Cv\nTo base (high): %CV\nTo 10 base (low): %to\nTo 10 base (high): %TO\n", 865, 784, 123, 16, 123, 16, "7b", 16, "7B", 16);
    overfprintf(stdout, "Bytes (signed int): %mi\nBytes (unsigned int): %mu\nBytes (double): %md\nBytes (float): %mf\n", +123, 456, 7.89, 10.15);
    char buffer[256];
    oversprintf(buffer, "BUFF\nRoman: %Ro\nZeckendorf: %Zr\nTo base (low): %Cv\nTo base (high): %CV\nTo 10 base (low): %to\nTo 10 base (high): %TO\n", 865, 784, 123, 16, 123, 16, "7b", 16, "7B", 16);
    buffer[255] = '\0';
    printf("\n%s", buffer);
    return success;
}

int oversprintf(char *buf, const char *format, ...){
    FILE *stream = tmpfile();
    if(!stream){
        print(wrong_input);
        return wrong_input;
    }
    va_list args;
    va_start(args, format);
    int length = strlen(format);
    if(!length){
        print(wrong_input);
        return fail;
    }
    int i = 0;
    int j = 0;
    int check = 0;
    int count = 0;
    char *result = (char*)malloc((length+1)*sizeof(char));
    if(result == NULL){
        va_end(args);
        print(memory_error);
        return fail;
    }
    for(int i = 0; i < length; i++){
        if(format[i] == '%'){
            if(check_flag(format[i+1], format[i+2]) == success){
                if(i != 0){
                    result[j] = '\0';
                    check = vfprintf(stream, result, args);
                    if(check < 0){
                        free(result);
                        va_end(args);
                        print(print_error);
                        return fail;
                    }
                    j = 0;
                    for (int k = 0; k < count; ++k){
                        if (format[i + 2] == 'f') {
                            va_arg(args, double);
                        } else {
                            va_arg(args, void*);
                        }
                        i += 2;
                    }
                }
                count = 0;
                change_flags(stream, &args, format[i+1], format[i+2]);
                i += 3;
                j = 0;
                for(int i = 0; i <= length; i++){
                    result[i] = 0;
                }
            }
            else{
                count++;
            }
        }
        result[j] = format[i];
        j++;
    }
    if (j != 0) {
        result[j] = '\0';
        check = vfprintf(stream, result, args);
        if (check < 0) {
            free(result);
            va_end(args);
            print(print_error);
            return fail;
        }
    }
    rewind(stream);
    char symbol;
    while((symbol = fgetc(stream)) != EOF){
        buf[i] = symbol;
        i++;
    }
    buf[i] = '\0';
    fclose(stream);
    free(result);
    va_end(args);
    return success;
}

int overfprintf(FILE *stream, const char *format, ...){
    if(!stream){
        print(wrong_input);
        return (wrong_input);
    }
    va_list args;
    va_start(args, format);
    int length = strlen(format);
    if(!length){
        print(wrong_input);
        return fail;
    }
    int i = 0;
    int j = 0;
    int check = 0;
    int count = 0;
    char *result = (char*)malloc((length+1)*sizeof(char));
    if(result == NULL){
        va_end(args);
        print(memory_error);
        return fail;
    }
    for(int i = 0; i < length; i++){
        if(format[i] == '%'){
            if(check_flag(format[i+1], format[i+2]) == success){
                if(i != 0){
                    result[j] = '\0';
                    check = vfprintf(stream, result, args);
                    if(check < 0){
                        free(result);
                        va_end(args);
                        print(print_error);
                        return fail;
                    }
                    j = 0;
                    for (int k = 0; k < count; ++k){
                        if (result[j - 1] == 'f'){
                            va_arg(args, double);
                        } 
                        else{
                            va_arg(args, void*);
                        } 
                        j++;
                    }
                }
                count = 0;
                change_flags(stream, &args, format[i+1], format[i+2]);
                i += 3;
                j = 0;
                for(int i = 0; i <= length; i++){
                    result[i] = 0;
                }
            }
            else{
                count++;
            }
        }
        result[j] = format[i];
        j++;
    }
    if (j != 0) {
        result[j] = '\0';
        check = vfprintf(stream, result, args);
        if (check < 0) {
            free(result);
            va_end(args);
            print(print_error);
            return fail;
        }
    }
    free(result);
    va_end(args);
    return success;
}

void change_flags(FILE *stream, va_list *args, char first, char second){
    char *result = NULL;
    int count = 0;
    int number = 0;
    int base = 0;
    int flag = 0;
    char *str_number;
    unsigned char *result_m = NULL;
    signed int number_m;
    unsigned int number_u;
    double number_d;
    float number_f;
    switch(first){
        case 'R':
            result = (char*)malloc(11*sizeof(char));
            if(result == NULL){
                print(memory_error);
                return;
            }
            if(flag_ro(va_arg(*args, int), &result) == wrong_input){
                print(wrong_input);
                free(result);
            }
            else{
                fprintf(stream, "%s", result);
                free(result);
            }
            break;
        case 'Z':
            count = flag_zr(va_arg(*args, unsigned int), &result);
            if(count == wrong_input){
                print(wrong_input);
                free(result);
            }
            else{
                fprintf(stream, "%s", result);
                free(result);
            }
            break;
        case 'C':
            switch(second){
                case 'v':
                    number = va_arg(*args, int);
                    base = va_arg(*args, int);
                    flag = 0;
                    if(flag_cv(number, base, &result, flag) == success){
                        fprintf(stream, "%s", result);
                        free(result);
                        break;
                    }
                case 'V':
                    number = va_arg(*args, int);
                    base = va_arg(*args, int);
                    flag = 1;
                    if(flag_cv(number, base, &result, flag) == success){
                        fprintf(stream, "%s", result);
                        free(result);
                        break;
                    }
            }
            break;
        case 't':
            str_number = va_arg(*args, char*);
            base = va_arg(*args, int);
            flag = 0;
            fprintf(stream, "%d", flag_to(str_number, base, flag));
            break;
        case 'T':
            str_number = va_arg(*args, char*);
            base = va_arg(*args, int);
            flag = 1;
            fprintf(stream, "%d", flag_to(str_number, base, flag));
            break;
        case 'm':
            switch(second){
                case 'i':
                    number_m = va_arg(*args, signed int);
                    if(flag_m(&number_m, sizeof(signed int), &result_m) == success){
                        for(int i = 0; i < sizeof(signed int)*8; i++){
                            if(i % 8 == 0){
                                fprintf(stream, " ");
                            }
                            fprintf(stream, "%u", result_m[i]);
                        }
                        free(result_m);
                    }
                    break;
                case 'u':
                    number_u = va_arg(*args, unsigned int);
                    if(flag_m(&number_u, sizeof(unsigned int), &result_m) == success){
                        for(int i = 0; i < sizeof(unsigned int)*8; i++){
                            if(i % 8 == 0){
                                fprintf(stream, " ");
                            }
                            fprintf(stream, "%u", result_m[i]);
                        }
                        free(result_m);
                    }
                    break;
                case 'd':
                    number_d = va_arg(*args, double);
                    if(flag_m(&number_d, sizeof(double), &result_m) == success){
                        for(int i = 0; i < sizeof(double)*8; i++){
                            if(i % 8 == 0){
                                fprintf(stream, " ");
                            }
                            fprintf(stream, "%u", result_m[i]);
                        }
                        free(result_m);
                    }
                    break;
                case 'f':
                    number_f = (float)va_arg(*args, double);
                    if(flag_m(&number_f, sizeof(float), &result_m) == success){
                        for(int i = 0; i < sizeof(float)*8; i++){
                            if(i % 8 == 0){
                                fprintf(stream, " ");
                            }
                            fprintf(stream, "%u", result_m[i]);
                        }
                        free(result_m);
                    }
                    break;
            }
            break;
    }
}

int check_flag(char first, char second){
    first = tolower(first);
    second = tolower(second);
    char *flags[9] = {"ro", "zr", "cv", "to", "mi", "mu", "md", "mf"};
    for(int i = 0; i < 8; i++){
        if(first == flags[i][0] && second == flags[i][1]){
            return success;
        }
    }
    return fail;
}

int flag_ro(int number, char **result){
    if (number <= 0 || number > 4000){
        return wrong_input;
    }
    char *roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    int i = 0;
    int j = 0;
    int k = 0;
    while (number > 0){
        if(number >= values[i]){
            if(i % 2 == 0){
                (*result)[j] = *roman[i];
                j ++;
                number -= values[i];
            }
            else{
                (*result)[j] = roman[i][k];
                j ++;
                (*result)[j] = roman[i][k+1];
                j ++;
                number -= values[i];
                k = 0;
            }
        }
        else{
            i++;
        }
    }
    (*result)[j] = '\0';
    return success;
}

int flag_zr(unsigned int number, char **result){
    if(number < 0){
        return wrong_input;
    }
    if(number == 0){
        (*result) = (char*)malloc(3*sizeof(char));
        if(*result == NULL){
            print(memory_error);
            return fail;
        }
        (*result)[0] = '0';
        (*result)[1] = '1';
        (*result)[2] = '\0';
        return 4;
    }
    int fib = 0;
    int length =  get_fib(number, &fib);;
    (*result) = (char*)malloc((length+2)*sizeof(char));
    if(*result == NULL){
        print(memory_error);
        return fail;
    }
    for(int i = 0; i < length + 2; i++){
        (*result)[i] = '0';
    }
    (*result)[length] = '1';
    (*result)[length+1] = '1';
    (*result)[length+2] = '\0';
    number -= fib;
    int count = get_fib(number, &fib);
    while(number > 0){
        (*result)[count] = '1';
        number -= fib;
        count = get_fib(number, &fib);
    }
    return length+2;
}

int get_fib(int number, int *fib){
    int fib_now = 1;
    int fib_prev = 1;
    int fib_next;
    int count = 0;
    while(number >= fib_now){
        fib_next = fib_now + fib_prev;
        fib_prev = fib_now;
        fib_now = fib_next;
        count++;
    }
    if(number != fib_now){
        *fib = fib_prev;
        count--;
        return count;
    }
    else{
        *fib = fib_now;
        return count;
    }
}

int flag_cv(int number, int base, char **result, int flag){
    if(base < 2 || base > 36){
        base = 10;
    }
    if(number == 0){
        (*result) = (char*)malloc(2*sizeof(char));
        if(*result == NULL){
            print(memory_error);
            return fail;
        }
        (*result)[0] = '0';
        (*result)[1] = '\0';
        return success;
    }
    int temp = number;
    int length = 0;
    while(temp != 0){
        temp /= base;
        length++;
    }
    (*result) = (char*)malloc((length + 1)*sizeof(char));
    if(*result == NULL){
        print(memory_error);
        return fail;
    }
    (*result)[length] = '\0';
    int digit = 0;
    for(int i = length - 1; i >= 0; i--){
        digit = number % base;
        if(digit < 10){
            (*result)[i] = digit + '0';
        }
        else{
            if(flag == 0){
                (*result)[i] = digit + 'a' - 10;
            }
            else{
                (*result)[i] = digit + 'A' - 10;
            }
        }
        number /= base;
    }
    return success;
}

int flag_to(char *number, int base, int flag){
    if(base < 2 || base > 36){
        base = 10;
    }
    if(number == "0"){
        return 0;
    }
    int length = strlen(number);
    int result = 0;
    int digit = 0;
    int power = 1;
    char temp;
    for (int i = length - 1; i >= 0; i--){
        if(isdigit(number[i])){
            digit = number[i] - '0';
        }
        else{
            if (flag == 0){
                digit = number[i] - 'a' + 10;
            }
            else{
                temp = tolower(number[i]);
                digit = temp - 'a' + 10;
            }
        }
        result += (digit*power);
        power *= base;
    }
    return result;
}

int flag_m(const void *ptr, size_t size, unsigned char **result){
    const unsigned char *byte = (unsigned char*)ptr;
    (*result) = (unsigned char*)malloc(size*8*sizeof(unsigned char));
    if(*result == NULL){
        print(memory_error);
        return fail;
    }
    (*result)[size-1] = '\0';
    int k = 0;
    for (size_t i = 0; i < size; i++){
        for(int j = 7; j >= 0; j--){
            (*result)[k] = (byte[i] >> j) & 1;
            k++;
        }
    }
    return success;
}

void print(int state){
    if(state == wrong_flag){
        printf("Wrong flag");
    }
    else if(state == wrong_input){
        printf("Wrong input");
    }
    else if(state == memory_error){
        printf("Memory error");
    }
    else if(state == print_error){
        printf("Print error");
    }
}