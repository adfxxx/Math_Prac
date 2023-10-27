#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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

int oversscanf(char *buf, const char *format, ...);
int overfscanf(FILE *stream, const char *format,...);
int change_flags(FILE *stream, va_list *args, char first, char second);
int get_file_string(FILE *stream, char **line);
int flag_ro(char *number);
int roman_numbers(char symbol);
unsigned int flag_zr(char *number);
int flag_cv(char *number, int base, int flag);
void print(int state);
int check_flag(char first, char second);

int main(){
    FILE *file = fopen("file.txt", "r");
    unsigned int number_zr;
    int result;
    int number_ro;
    int num;
    int number_cv;
    int number_cV;
    int base = 16;
    result = overfscanf(file, "%Ro %Zr %Cv %CV", &number_ro, &number_zr, &number_cv, base, &number_cV, base);
    if(result == success){
        printf("Roman: %d, zeckendorf: %u, in base 10(low): %d, in base 10(high): %d\n", number_ro, number_zr, number_cv, number_cV);
    }
    else{
        printf("Error");
    }
    char *buff = "CMXI 100010100010011 7b 23A";
    result = oversscanf(buff, "%Ro %Zr %Cv %CV", &number_ro, &number_zr, &number_cv, base, &number_cV, base);
    if(result == success){
         printf("Roman: %d, zeckendorf: %u, in base 10(low): %d, in base 10(high): %d\n", number_ro, number_zr, number_cv, number_cV);
    }
    else{
        printf("Error");
    }
    return success;
}

int oversscanf(char *buf, const char *format, ...){
    FILE *stream = tmpfile();
    if(!stream){
        print(wrong_input);
        return wrong_input;
    }
    int leng = strlen(buf);
    int i = 0;
    while(buf[i] != '\0'){
        fputc(buf[i], stream);
        i++;
    }
    rewind(stream);
    va_list args;
    va_start(args, format);
    int length = strlen(format);
    if(!length){
        print(wrong_input);
        return fail;
    }
    int j = 0;
    int check = 0;
    char *result = (char*)malloc((length+1)*sizeof(char));
    if(result == NULL){
        va_end(args);
        print(memory_error);
        return fail;
    }
    void *flag;
    for (int i = 0; i < length; i++){
        if(format[i] == '%'){
            if(check_flag(format[i+1], format[i+2]) == success){
                if(j != 0){
                    result[j] = 0;
                    check = vfscanf(stream, result, args);
                    if(check == EOF){
                        free(result);
                        va_end(args);
                        print(print_error);
                        return fail;
                    }
                    for(int j = 0; j < check; j++){
                        flag = va_arg(args, void*);
                    }
                }
                if(change_flags(stream, &args, format[i+1], format[i+2]) == fail){
                    return fail;
                }
                j = 0;
                i+= 3;
            }
        }
        result[j] = format[i];
        j++;
    }
    if (j != 0) {
        result[j] = 0;
        check = vfscanf(stream, result, args);
        if (check < 0) {
            free(result);
            va_end(args);
            print(print_error);
            return fail;
        }
    }
    free(result);
    fclose(stream);
    remove("tmpfile");
    va_end(args);
    return success;
}

int overfscanf(FILE *stream, const char *format,...){
    if(!stream){
        print(wrong_input);
        return (wrong_input);
    }
    va_list args;
    va_start(args, format);
    int length = strlen(format);
    char *result = (char*)malloc((length+1)*sizeof(char));
    if(result == NULL){
        va_end(args);
        print(memory_error);
        return fail;
    }
    int j = 0;
    int check = 0;
    void *flag;
    for (int i = 0; i < length; i++){
        if(format[i] == '%'){
            if(check_flag(format[i+1], format[i+2]) == success){
                if(j != 0){
                    result[j] = 0;
                    check = vfscanf(stream, result, args);
                    if(check == EOF){
                        free(result);
                        va_end(args);
                        print(print_error);
                        return fail;
                    }
                    for(int j = 0; j < check; j++){
                        flag = va_arg(args, void*);
                    }
                }
                if(change_flags(stream, &args, format[i+1], format[i+2]) == fail){
                    return fail;
                }
                j = 0;
                i+= 3;
            }
        }
        result[j] = format[i];
        j++;
    }
    if (j != 0) {
        result[j] = 0;
        check = vfscanf(stream, result, args);
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

int change_flags(FILE *stream, va_list *args, char first, char second){
    char *line = NULL;
    if(get_file_string(stream, &line) == fail){
        return fail;
    }
    unsigned int *result_zr;
    unsigned int number_zr;
    int *result_ro;
    int number_ro = 0;
    int *result_cv;
    int base = 0;
    int number_cv;
    int flag;
    switch(first){
        case 'R':
            result_ro = va_arg(*args, int*);
            number_ro = flag_ro(line);
            *result_ro = number_ro;
            break;
        case 'Z':
            result_zr = va_arg(*args, unsigned int*);
            number_zr = flag_zr(line);
            *result_zr = number_zr;
            break;
        case 'C':
            switch(second){
                case 'v':
                    result_cv = va_arg(*args, int*);
                    base = va_arg(*args, int);
                    flag = 0;
                    number_cv = flag_cv(line, base, flag);
                    *result_cv = number_cv;
                    break;
                case 'V':
                    result_cv = va_arg(*args, int*);
                    base = va_arg(*args, int);
                    flag = 1;
                    number_cv = flag_cv(line, base, flag);
                    *result_cv = number_cv;
                    break;
            }
            break;
    }
    free(line);
    return success;
}

int get_file_string(FILE *stream, char **line){
    *line = (char*)malloc(2*sizeof(char));
    if(*line == NULL){
        print(memory_error);
        return fail;
    }
    int length = 2;
    char *temp_line;
    char symbol = fgetc(stream);
    while (symbol == '\n' || symbol == '\t' || symbol == ' '){
        symbol = fgetc(stream);
    }
    int i = 0;
    while (symbol != '\n' && symbol != '\t' && symbol != ' ' && symbol != EOF){
        if(length <= i){
            length *= 2;
            temp_line = (char*)realloc(*line, length*sizeof(char));
            if(temp_line == NULL){
                free(line);
                print(memory_error);
                return fail;
            }
            *line = temp_line;
        }
        (*line)[i] = symbol;
        i++;
        symbol = fgetc(stream);
    }
    (*line)[i] = '\0';
    return success;
}

int flag_ro(char *number){
    int result = 0;
    int prev = 0;
    int current = 0;
    for (int i = 0; number[i] != '\0'; i++){
        current = roman_numbers(number[i]);
        if (current > prev){
            result += current - 2*prev;
        }
        else{
            result += current;
        }
        prev = current;
    }
    return result;
}

int roman_numbers(char symbol){
    switch(symbol) {
        case 'I':
            return 1;
        case 'V':
            return 5;
        case 'X':
            return 10;
        case 'L':
            return 50;
        case 'C':
            return 100;
        case 'D':
            return 500;
        case 'M':
            return 1000;
        default:
            return -1;
    }
}

unsigned int flag_zr(char *number){
    int length = strlen(number);
    unsigned int result = 0;
    unsigned int fib_1 = 0;
    unsigned int fib_2 = 1;
    unsigned int fib;
    int i = 0;
    while (i < length - 1){
        fib = fib_1+fib_2;
        if(number[i] == '1'){
            result += fib;
        }
        fib_1 = fib_2;
        fib_2 = fib;
        i++;
    }
    return result;
}

int flag_cv(char *number, int base, int flag){
    if(base < 2 && base > 36){
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

int check_flag(char first, char second){
    first = tolower(first);
    second = tolower(second);
    char *flags[4] = {"ro", "zr", "cv"};
    for(int i = 0; i < 3; i++){
        if(first == flags[i][0] && second == flags[i][1]){
            return success;
        }
    }
    return fail;
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