#include "validation.h"

int is_valid_digit(char *str){
    int i = 0;
    while(str[i] != '\0'){
        if(!isdigit(str[i])){
            return 0;
        }
        i++;
    }
    if(!i){
        return 0;
    }
    else{
        return 1;
    }
}

int is_valid_double(char *str){
    int dots = 0;
    int index = 0;
    while(str[index] != '\0'){
        if(!isdigit(str[index])){
            if(str[index] == '.' && dots == 0){
                dots++;
            }
            else{
                return 0;
            }
        }
        index++;
    }
    return (index == 0) ? 0 : 1;
}

int is_valid_time(char *str){
    if(strlen(str) != 19){
        return 0;
    }

    for(int i = 0; i < 19; i++){
        if(i == 4 || i == 7){
            if(str[i] != '-'){
                return 0;
            }
        }
        else if(i == 10){
            if(str[i] != 'T'){
                return 0;
            } 
        }
        else if(i == 13 || i == 16){
            if(str[i] != ':'){
                return 0;
            }
        }
        else{
            if(!isdigit(str[i]) && str[i] != '\n'){
                return 0;
            }
        }
    }
    int year, month, day, hour, minute, second;
    sscanf(str, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    if(year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59){
        return 0;
    }

    int days_in_mnth[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(day > days_in_mnth[month-1]){
        return 0;
    }
    return 1;
}

int is_leap_year(int year){
    return(year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

Status is_valid_data(char *heap_type, char *set_type, char *min_time, char *max_time, char *start_time, char *end_time, char *overload_coeff, char *count_deps){
    if(!heap_type || !set_type || !min_time || !max_time || !start_time || !end_time || !overload_coeff || !count_deps){
        return wrong_argument;
    }
    
    Heap_type h_t;
    Status status = get_heap_type(heap_type, &h_t);
    if(h_t == ERROR_HEAP || status != success){
        return wrong_data;
    }
    
    Set_type s_t;
    get_set_type(set_type, &s_t);
    if(s_t == ERROR_SET || status != success){
        return wrong_data;
    }
    
    if(!is_valid_time(start_time) || !is_valid_time(end_time)){
        return wrong_data;
    }
    if(!is_valid_digit(min_time) || !is_valid_digit(max_time)){
        return wrong_data;
    }
    if(!is_valid_double(overload_coeff)){
        return wrong_data;
    }
    if(!is_valid_digit(count_deps)){
        return wrong_data;
    }
    
    if(strcmp(start_time, end_time) > 0){
        return wrong_data;
    }
    return success;
}

Status get_heap_type(char *str, Heap_type *type){
    if(!str){
        return wrong_argument;
    }

    if(!strcmp(str, "BinaryHeap")){
        *type = Binary;
    }
    else if(!strcmp(str, "BinomialHeap")){
        *type = Binomial;
    }
    else if(!strcmp(str, "FibonacciHeap")){
        *type = Fibonacci;
    }
    else if(!strcmp(str, "LeftistHeap")){
        *type = Leftist;
    }
    else if(!strcmp(str, "SkewHeap")){
        *type = Skew;
    }
    else if(!strcmp(str, "Treap")){
        *type = Treap;
    }
    else{
        *type = ERROR_HEAP;
    }
    return success;
}

Status get_set_type(char *str, Set_type *type){
    if(!str){
        return wrong_argument;
    }

    if(!strcmp(str, "HashSet")){
        *type = Hash;
    }
    else if(!strcmp(str, "DynamicArray")){
        *type = Array;
    }
    else if(!strcmp(str, "BinarySearchTree")){
        *type = Bst;
    }
    else if(!strcmp(str, "Trie")){
        *type = Trie;
    }
    else{
        *type = ERROR_SET;
    }
    return success;
}

void free_with_amount(int amount,...){
    va_list list;
    va_start(list, amount);

    for(int i = 0; i < amount; i++){
        void *elem = va_arg(list, void*);
        if(elem){
            free(elem);
        }
    }
    va_end(list);
}