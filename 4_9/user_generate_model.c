#include <stdio.h>
#include <stdlib.h>
#include "functions/validation.h"

char *read_str(FILE *console);

int main(){
    char *requests_types[] = {"BinaryHeap\n", "BinomialHeap\n", "FibonacciHeap\n", "SkewHeap\n", "LeftistHeap\n", "Treap\n"};
    char *deps_types[] = {"HashSet\n", "DynamicArray\n", "BinarySearchTree\n", "Trie\n"};
    FILE *file = fopen("info.txt", "w");
    if(!file){
        print_error(file_for_info_is_not_open);
        return file_for_info_is_not_open;
    }
    FILE *console = fopen("/dev/tty", "r");
    if(!console){
        print_error(error);
        return error;
    }

    printf("Starting generation.\n");

    printf("Choose a structure for requests storage (enter a number):\n");
    for(int i = 0; i < 6; i++){
        printf("\t%d. %s", i+1, requests_types[i]);
    }
    int type;
    scanf("%d", &type);
    if(type < 1 || type > 6){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fputs(requests_types[type-1], file);

    printf("\nChoose a structure for departments storage (enter a number):\n");
    for(int i = 0; i < 4; i++){
        printf("\t%d. %s", i+1, deps_types[i]);
    }
    scanf("%d", &type);
    if(type < 1 || type > 4){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fputs(deps_types[type-1], file);

    printf("\nEnter time of starting a model (format: year-month-dayThour:minute:second):\n");
    char *str = read_str(console);
    if(!is_valid_time(str)){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fprintf(file, "%s\n", str);

    printf("\nEnter time of ending a model (format: year-month-dayThour:minute:second):\n");
    str = read_str(console);
    if(!is_valid_time(str)){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fprintf(file, "%s\n", str);

    printf("\nEnter min time for handling a request:\n");
    str = read_str(console);
    if(!is_valid_digit(str)){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fprintf(file, "%s\n", str);

    printf("\nEnter max time for handling a request:\n");
    str = read_str(console);
    if(!is_valid_digit(str)){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fprintf(file, "%s\n", str);

    printf("\nEnter number of departments (from 1 to 100):\n");
    str = read_str(console);
    int num = atoi(str);
    if(!is_valid_digit(str) || num > 100 || num < 1){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fprintf(file, "%s\n", str);

    char *str_2 = NULL;
    for(int i = 0; i < num; i++){
        printf("\nEnter a department id:\n");
        str = read_str(console);
        if(!is_valid_digit(str)){
            print_error(wrong_input);
            fclose(file);
            return wrong_input;
        }
        printf("\nEnter a number of operators for department:\n");
        str_2 = read_str(console);
        if(!is_valid_digit(str_2) || atoi(str_2) < 10 || atoi(str_2) > 50){
            print_error(wrong_input);
            fclose(file);
            return wrong_input;
        }
        fprintf(file, "%s %s\n", str, str_2);
    }
    free(str_2);

    printf("\nEnter an overload coeff (double):\n");
    str = read_str(console);
    if(!is_valid_double(str)){
        print_error(wrong_input);
        fclose(file);
        return wrong_input;
    }
    fprintf(file, "%s", str);

    printf("Success.\n");
    fclose(file);
    fclose(console);
    free(str);
    return success;
}   

char *read_str(FILE *console){
    char *str;
    size_t size = 0; 
    getline(&str, &size, console);
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    return str;
}
