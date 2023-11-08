#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors{
    success = 1,
    memory_error = -1
};

enum comparison{
    first = 1,
    second = 2,
    equal = 3,
    not_equal =4
};

typedef struct{
    char *string;
    int length;
} String;

int create(String *example, const char *str);
void delete(String *example);
int compare_strings(const String *string_1, const String *string_2);
int equality(const String *string_1, const String *string_2);
void copy_to_exist(const String *copy, String *result);
void copy_to_new(const String *copy, String *new);
int concate(String *first, const String *second);
void print(int state);

int main(){
    char *str = "";
    String example;
    int result = create(&example, str);
    if(result == memory_error){
        print(memory_error);
        return memory_error;
    }
    else{
        printf("Your created string: %s\n", example.string);
    }

    delete(&example);
    printf("Your string after deleting: %s\n", example.string);

    String string_1;
    str = "abcd";
    result = create(&string_1, str);
    if(result == memory_error){
        print(memory_error);
        return memory_error;
    }
    String string_2;
    str = "rtyui";
    result = create(&string_2, str);
    if(result == memory_error){
        print(memory_error);
        return memory_error;
    }
    result = compare_strings(&string_1, &string_2);
    if(result == first){
        printf("First string (%s) is greater than second (%s)\n", string_1.string, string_2.string);
    }
    else if(result == second){
        printf("Second string (%s) is greater than first (%s)\n", string_2.string, string_1.string);
    }
    else{
        printf("First string (%s) is equal to second (%s)\n", string_1.string, string_2.string);
    }

    result = equality(&string_1, &string_2);
    if(result == equal){
        printf("First string (%s) is equal to second (%s)\n", string_1.string, string_2.string);
    }
    else{
        printf("First string (%s) is not equal to second (%s)\n", string_1.string, string_2.string);
    }
    char *rtr = "";

    copy_to_exist(&string_1, &string_2);
    printf("Copied string: %s\n", string_2.string);

    String new;

    copy_to_new(&string_1, &new);
    printf("Copied new string: %s\n", new.string);

    result = concate(&new, &string_2);
    if(result == memory_error){
        print(memory_error);
        return memory_error;
    }
    else{
        printf("Concated strings: %s", new.string);
    }

    delete(&string_1);
    delete(&string_2);
    delete(&new);
} 

int concate(String *first, const String *second){
    int length = first->length + second->length;
    char *temp = (char*)realloc(first->string, length+1);
    if(temp == NULL){
        return memory_error;
    }
    first->string = temp;
    strcat(first->string, second->string);
    first->length = length;
    return success;
}

void copy_to_new(const String *copy, String *new){
    int length = copy->length;
    new->length = length;
    new->string = (char*)malloc((length+1)*sizeof(char));
    strcpy(new->string, copy->string);
}

void copy_to_exist(const String *copy, String *result){
    int length = copy->length;
    result->length = length;
    if(result->string != NULL){
        free(result->string);
        result->string = NULL;
    }
    result->string = (char*)malloc((length+1)*sizeof(char));
    strcpy(result->string, copy->string);
}

int equality(const String *string_1, const String *string_2){
    return(strcmp(string_1->string, string_2->string) == 0) ? equal : not_equal;
}

int compare_strings(const String *string_1, const String *string_2){
    int length = string_1->length - string_2->length;
    if(length != 0){
        if (length > 0){
            return first;
        }
        else{
            return second;
        }
    }
    int result = strcmp(string_1->string, string_2->string);
    if(result == 0){
        return equal;
    }
    else if(result > 0){
        return first;
    }
    else{
        return second;
    }    
}

void delete(String *example){
    if(example->string != NULL){
        free(example->string);
        example->string = NULL;
    }
}

int create(String *example, const char *str){
    example->length = strlen(str);
    example->string = (char*)malloc((example->length + 1)*sizeof(char));
    if(example->string == NULL){
        return memory_error;
    }
    strcpy(example->string, str);
    return success;
}

void print(int state){
    if(state == memory_error){
        printf("Memory error\n");
    }
}