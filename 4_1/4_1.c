#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4
};

typedef struct Elem{
    char *macro;
    char *subst;
    int hash;
    struct Elem *next;
    struct Elem *prev;
}Elem;

typedef struct Elements{
    struct Elem *head;
    struct Elem *tail;
}Elements;

typedef struct{
    int size;
    int number;
    struct Elements **elements;
}Hash;

void printHashTable(Hash *table);
int insert(Hash *table, char *macro, char *subst);
int hash_function(char *macro, Hash *table);
int get_def_name(char *macro);
int read_file(FILE *input, Hash *table);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 2){
        print(wrong_input);
        return wrong_input;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }

    Hash table;
    table.size = 128;
    table.number = 0;
    table.elements = (Elements**)malloc(table.size*sizeof(Elements*));
    if(table.elements == NULL){
        print(memory_error);
        fclose(input);
        return memory_error;
    }

    for(int i = 0; i < table.size; i++){
        table.elements[i] = (Elements*)malloc(sizeof(Elements));
        if(table.elements[i] == NULL){
            print(memory_error);
            free(table.elements);
            fclose(input);
            return memory_error;
        }
        table.elements[i]->head = NULL;
        table.elements[i]->tail = NULL;
    }
    int result = read_file(input, &table);
    if(result == memory_error){
        print(memory_error);
        fclose(input);
        for(int i = 0; i < table.size; i++){
            free(table.elements[i]);
        }
        free(table.elements);
        return memory_error;
    }

    printHashTable(&table);
    for(int i = 0; i < table.size; i++){
        free(table.elements[i]);
    }
    free(table.elements);
    fclose(input);
    return success;
}

void printHashTable(Hash *table) {
    for (int i = 0; i < table->size; i++) {
        printf("Bucket %d: ", i);
        Elem *current = table->elements[i]->head;
        while (current != NULL) {
            printf("(%s, %s) ", current->macro, current->subst);
            current = current->next;
        }
        printf("\n");
    }
}

int insert(Hash *table, char *macro, char *subst){
    int index = hash_function(macro, table);
    Elem *new_elem = (Elem*)malloc(sizeof(Elem));
    if(new_elem == NULL){
        return memory_error;
    }
    new_elem->macro = strdup(macro);
    new_elem->subst = strdup(subst);
    new_elem->hash = hash_function(macro, table);
    new_elem->next = NULL;
    new_elem->prev = NULL;
    if(table->elements[index]->head == NULL){
        table->elements[index]->head = new_elem;
    }
    else{
        table->elements[index]->tail->next = new_elem;
        new_elem->prev = table->elements[index]->tail;
    }
    table->elements[index]->tail = new_elem;
    table->number++;
    return success;
}

int hash_function(char *macro, Hash *table){
    int macro_num = get_def_name(macro);
    return (macro_num % table->size);
}

int get_def_name(char *macro){
    int result = 0;
    int len = strlen(macro);
    char symbol;
    int digit;
    for(int i = 0; i < len; i++){
        symbol = macro[i];
        if('0' <= symbol && symbol <= '9'){
            digit = symbol - '0';
        }
        else if('A' <= symbol && symbol <= 'Z'){
            digit = symbol - 'A' + 10;
        }
        else if('a' <= symbol && symbol <= 'z'){
            digit = symbol - 'a' + 36;
        }
        result = result*62 + digit;
    }
    return result;
}

int read_file(FILE *input, Hash *table){
    char *line = NULL;
    size_t len = 0;
    int size = 100;
    char *macro = NULL;
    char *subst = NULL;
    char *temp;
    int i = 0;
    int k = 0;
    int result;
    while(getline(&line, &len, input) != -1){
        macro = (char*)malloc(size*sizeof(char));
        if(macro == NULL){
            return memory_error;
        }
        subst = (char*)malloc(size*sizeof(char));
        if(subst == NULL){
            free(macro);
            return memory_error;
        }
        if(!strncmp(line, "#define", 7)){
            i = 7;
            k = 0;
            while(line[i] == ' '){
                i++;
            }
            while(line[i] != ' ' && line[i] != '\n'){
                macro[k] = line[i];
                k++;
                i++;
                if(k == size){
                    size *= 2;
                    temp = (char*)realloc(macro, size*sizeof(char));
                    if(temp == NULL){
                        free(macro);
                        free(subst);
                        return memory_error;
                    }
                    macro = temp;
                }
            }
            macro[k] = '\0';
            k = 0;
            while(line[i] == ' '){
                i++;
            }

            while(line[i] != ' ' && line[i] != '\n'){
                subst[k] = line[i];
                k++;
                i++;
                if(k == size){
                    size *= 2;
                    temp = (char*)realloc(subst, size*sizeof(char));
                    if(temp == NULL){
                        free(macro);
                        free(subst);
                        return memory_error;
                    }
                    subst = temp;
                }
            }
            subst[k] = '\0';

            result = insert(table, macro, subst);
            if(result == memory_error){
                free(macro);
                free(subst);
                return memory_error;
            }
            free(macro);
            free(subst);
        }
    }
    free(line);
    return success;
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