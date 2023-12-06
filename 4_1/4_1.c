#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

enum errors{
    same = 2,
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4,
    chain_error = -5,
    is_not_open_output = -6
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
    struct Elements **elements;
}Hash;

void free_table(Hash *table);
void printHashTable(Hash *table);
int resize(Hash *table);
int get_size(int size);
int check_prime(int number);
int check_length(Hash *table);
int insert(Hash *table, int hash_func, char *macro, char *subst);
int insert_to_chain(Elements *chain, int hash_func, char *macro, char *subst);
Elem *find_in_chain(Elements *chain, char *macro);
int hash_function(char *macro);
int read_file(FILE *input, Hash *table);
int replace_words(Hash *table, char *line, FILE* output);
Elem *find_in_table(Hash *table, char *macro);
int check_macro(char *macro);
int check_str(char *line);
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
        free_table(&table);
        return memory_error;
    }
    else if(result == chain_error){
        print(chain_error);
        fclose(input);
        free_table(&table);
        return chain_error;
    }
    free_table(&table);
    fclose(input);
    printf("Successful changing!\n");
    return success;
}

void free_table(Hash *table){
    for(int i = 0; i < table->size; i++){
        free(table->elements[i]);
    }
    free(table->elements);
}

void printHashTable(Hash *table) {
    for (int i = 0; i < table->size; i++) {
        Elem *current = table->elements[i]->head;
        while(current != NULL) {
            printf("Bucket %d: ", i+1);
            printf("(%s, %s)\n", current->macro, current->subst);
            current = current->next;
        }
    }
}

int resize(Hash *table){
    Hash new_table;
    int new_size = get_size(table->size);
    new_table.size = new_size;
    new_table.elements = (Elements**)malloc(new_size*sizeof(Elements*));
    if(new_table.elements == NULL){
        return memory_error;
    }
    for(int i = 0; i < new_size; i++){
        new_table.elements[i] = (Elements*)malloc(sizeof(Elements));
        if(new_table.elements[i] == NULL){
            return memory_error;
        }
        new_table.elements[i]->head = NULL;
        new_table.elements[i]->tail = NULL; 
    }
    int result;
    int index;
    for(int i = 0; i < table->size; i++){
        Elem *cur = table->elements[i]->head;
        while(cur != NULL){
            index = cur->hash % new_size;
            result = insert_to_chain(new_table.elements[index], cur->hash, cur->macro, cur->subst);
            if(result == memory_error){
                free_table(&new_table);
                return memory_error;
            }
            cur = cur->next;
        }
    }
    free_table(table);
    *table = new_table;
    return success;
}

int get_size(int size){
    int number = size + 1;
    while(check_prime(number) != success){
        number++;
    }
    return number;
}

int check_prime(int number){
    for (int i = 2; i < number; ++i ){
        if (number % i == 0){
            return fail;
        }
    }
    return success;
}

int check_length(Hash *table){
    int min_chain = table->size;
    int max_chain = 0;
    int length = 0;
    for(int i = 0; i < table->size; i++){
        length = 0;
        Elem *cur = table->elements[i]->head;
        while(cur != NULL){
            length++;
            cur = cur->next;
        }
        if(length < min_chain && length != 0){
            min_chain = length;
        }
        if(length > max_chain){
            max_chain = length;
        }
    }
    if(min_chain == table->size && max_chain >= 128){
        return chain_error;
    }
    else if(max_chain >= 2*min_chain){
        return fail;
    }
    else{
        return success;
    }
}

int insert(Hash *table, int hash_func, char *macro, char *subst){
    int index = hash_func % table->size; //остаток
    int result = insert_to_chain(table->elements[index], hash_func, macro, subst);
    if(result == memory_error){
        return memory_error;
    }
    if(check_length(table) == fail){
        if(resize(table) == memory_error){
            return memory_error;
        }
    }
    else if(check_length(table) == chain_error){
        return chain_error;
    }
    return success;
}

int insert_to_chain(Elements *chain, int hash_func, char *macro, char *subst){
    Elem *check_elem = find_in_chain(chain, macro);
    if(check_elem != NULL){
        free(check_elem->subst);
        check_elem->subst = strdup(subst);
        if(check_elem->subst == NULL){
            return memory_error;
        }
        return same;
    }

    Elem *new_elem = (Elem*)malloc(sizeof(Elem));
    if(new_elem == NULL){
        return memory_error;
    }
    new_elem->macro = strdup(macro);
    if(new_elem->macro == NULL){
        return memory_error;
    }
    new_elem->subst = strdup(subst);
    if(new_elem->subst == NULL){
        return memory_error;
    }
    new_elem->hash = hash_func; //переведенное число
    new_elem->next = NULL;
    new_elem->prev = NULL;
    if(chain->head == NULL){
        chain->head = new_elem;
    }
    else{
        chain->tail->next = new_elem;
        new_elem->prev = chain->tail;
    }
    chain->tail = new_elem;
    return success;
}

Elem *find_in_chain(Elements *chain, char *macro){
    Elem *cur = chain->head;
    while(cur != NULL){
        if(!strcmp(cur->macro, macro)){
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int hash_function(char *macro){ 
    int result = 0;
    int power = 1;
    int len = strlen(macro);
    char symbol;
    int digit;
    for(int i = len-1; i >= 0; i--){
        symbol = macro[i];
        if('0' <= symbol && symbol <= '9'){
            digit = symbol - '0';
        }
        else if('A' <= symbol && symbol <= 'Z'){
            digit = symbol - 'A' + 11;
        }
        else if('a' <= symbol && symbol <= 'z'){
            digit = symbol - 'a' + 37;
        }
        result += (digit*power);
        power *= 62;
    }
    return result;
}

int read_file(FILE *input, Hash *table){
    FILE *output = fopen("output.txt", "w");
    if(!output){
        return is_not_open_output;
    }
    char *line = NULL;
    size_t len = 0;
    int size = 100;
    char *macro = NULL;
    char *subst = NULL;
    char *temp;
    int i = 0;
    int k = 0;
    int result;
    int flag = 1;
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
        if(check_str(line) == success && flag != 0){
            flag = 1;
        }
        else{
            flag = 0;
        }
        if(flag){
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
                        fclose(output);
                        free(line);
                        return memory_error;
                    }
                    macro = temp;
                }
            }
            macro[k] = '\0';
            if(check_macro(macro) == fail){
                free(macro);
                free(subst);
                continue;
            }
            k = 0;
            i++;
            int flag = 0;
            while(line[i] != '\0' && line[i] != '\n' && line[i] != EOF){
                subst[k] = line[i];
                k++;
                i++;
                if(k == size){
                    size *= 2;
                    temp = (char*)realloc(subst, size*sizeof(char));
                    if(temp == NULL){
                        free(macro);
                        free(subst);
                        fclose(output);
                        free(line);
                        return memory_error;
                    }
                    subst = temp;
                }
            }
            subst[k] = '\0';
            int hash_func = hash_function(macro);
            result = insert(table, hash_func, macro, subst);
            if(result == memory_error){
                free(macro);
                free(subst);
                fclose(output);
                free(line);
                return memory_error;
            }
            else if(result == chain_error){
                free(macro);
                fclose(output);
                free(line);
                free(subst);
                return chain_error;
            }
            free(macro);
            free(subst);
        }
        else{
            result = replace_words(table, line, output);
            if(result == memory_error){
                free(line);
                fclose(output);
                return memory_error;
            }
        }
    }
    fclose(output);
    free(line);
    return success;
}

int replace_words(Hash *table, char *line, FILE* output){
    int i = 0;
    int j = 0;
    int size = 100;
    char *word = NULL;
    char *new_word = NULL;
    int subst_len;
    while(line[i] != EOF && line[i] != '\n' && line[i] != '\0'){
        j = 0;
        size = 100;
        word = (char*)malloc(size*sizeof(char));
        if(word == NULL){
            return memory_error;
        }
        while(line[i] == ' '){
            fprintf(output, " ");
            i++;
        }
        while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n' && line[i] != EOF){
            word[j] = line[i];
            i++;
            j++;
            if(j == size - 1){
                char *temp = (char*)realloc(word, size*2*sizeof(char));
                if(temp == NULL){
                    free(word);
                    return memory_error;
                }
                size *= 2;
                word = temp;
            }
        }
        word[j] = '\0';
        Elem *change = find_in_table(table, word);
        if(change != NULL){
            subst_len = strlen(change->subst);
            new_word = (char*)malloc((subst_len+1)*sizeof(char));
            if(new_word == NULL){
                return memory_error;
            }
            strncpy(new_word, change->subst, subst_len);
            new_word[subst_len] = '\0';
            fprintf(output, "%s", new_word);
            free(new_word);
        }
        else{
            fprintf(output, "%s", word);
        }
        if(line[i] == '\n'){
            fprintf(output, "\n");
        }
        free(word);
    }
    return success;
}

Elem *find_in_table(Hash *table, char *macro){
    for(int i = 0; i < table->size; i++){
        Elem *found = find_in_chain(table->elements[i], macro);
        if(found != NULL){
            return found;
        }
    }
    return NULL;
}

int check_macro(char *macro){
    int length = strlen(macro);
    for(int i = 0; i < length; i++){
        if(!isdigit(macro[i]) && !isalpha(macro[i])){
            return fail;
        }
    }
    return success;
}

int check_str(char *line){
    if(strncmp(line, "#define", 7) != 0){
        return fail;
    }
    int length = strlen(line);
    int space_count = 0;
    for(int i = 0; i < length; i++){
        if(line[i] == ' '){
            space_count++;
        }
    }
    if(space_count != 2){
        return fail;
    }
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
    else if(state == chain_error){
        printf("Memory error: too many elements in one chain.\n");
    }
    else if(state == is_not_open_output){
        printf("Output file is not open.\n");
    }
}