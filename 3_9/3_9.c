#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    is_not_open_output = -4,
    fail = -5
};

enum options{
    menu = 2,
    after_function = 3
};

typedef struct Node{
    char *word;
    int count;
    struct Node *left;
    struct Node *right;
}Node;

void free_node(Node *node);
int write_to_file(FILE *output, Node *root, char *separator, int flag);
int get_tree_depth(Node *root);
int get_long_short_word(Node *root, char **long_word, char **short_word);
void get_words(Node *root, Node **words, int *count, int number);
int find_word(Node *root, char *word);
int change_command(Node *root, char *separator);
int total_words(Node *root);
void print_tree(Node *root, int level);
Node *create_node(char *word);
Node *insert(Node *root, char *word);
int read_file(FILE *input, char *separator, Node **root);
int is_sep(const char symbol, char *separator);
void print(int state);

int main(int argc, char *argv[]){
    if(argc < 3){
        print(wrong_input);
        return wrong_input;
    }
    int size = argc-1;
    char *separator = (char*)malloc(size*sizeof(char));
    if(separator == NULL){
        print(memory_error);
        return memory_error;
    }
    int j = 0;
    for(int i = 2; i < argc; i++){
        separator[j] = argv[i][0];
        j++;
    }
    separator[j] = '\0';
    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }

    int result;
    Node *root = NULL;
    result = read_file(input, separator, &root);
    if(result == memory_error){
        fclose(input);
        free(separator);
        print(memory_error);
        return memory_error;
    }

    result = change_command(root, separator);
    if(result == memory_error){
        print(memory_error);
        free(separator);
        fclose(input);
        return memory_error;
    }
    free_node(root);
    printf("\nGoodbye!");
    free(separator);
    fclose(input);
    return success;
}

int change_command(Node *root, char *separator){
    int option = 0;
    int length = total_words(root);
    int number;
    Node **words;
    while(option != 7){
        option = 0;
        print(menu);
        if(!scanf("%d", &option) || option < 1 || option > 7){
            print(wrong_input);
            print(menu);
            fflush(stdin);
            while(!scanf("%d", &option) || option < 1 || option > 7){
                fflush(stdin);
                print(wrong_input);
                print(menu);
            }
        }
        char *word = NULL;
        size_t len = 0;
        int check;
        int count;
        char *long_word;
        char *short_word;
        int depth;
        FILE *output;
        switch(option){
            case 1:
            fflush(stdin);
            printf("Enter a word to search: ");
            check = getline(&word, &len, stdin);
            if(check != -1){
                if(word[check - 1] == '\n'){
                    word[check - 1] = '\0';
                }
            }
            count = find_word(root, word);
            printf("The word '%s' is found %d times in the file.\n", word, count);
            free(word);

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 2:
            fflush(stdin);
            printf("Enter a number of words to show: ");
            if(!scanf("%d", &number) || number > length){
                print(wrong_input);
                break;
            }
            int count = 0;
            words = (Node**)malloc(number*sizeof(Node*));
            if(words == NULL){
                return memory_error;
            }
            for(int i = 0; i < number; i++){
                words[i] = NULL;
            }
            get_words(root, words, &count, number);
            printf("\nTop %d the most frequent words:\n", number);
            for(int i = 0; i < number; i++){
                printf("%d. %s\n", i+1, words[i]->word);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 3:
            long_word = NULL;
            short_word = NULL;
            check = get_long_short_word(root, &long_word, &short_word);
            if(check == fail){
                printf("\nThere are no words in the system\n");
                break;
            }
            if(long_word != NULL){
                printf("\nThe longest word: %s\n", long_word);
                free(long_word);
            }
            if(short_word != NULL){
                printf("The shortest word: %s\n", short_word);
                free(short_word);
            }

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 4:
            depth = get_tree_depth(root);
            printf("\nTree depth: %d\n", depth);

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 5:
            printf("\nTree:\n");
            print_tree(root, 0);

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 6:
            output = fopen("output.txt", "w");
            if(!output){
                print(is_not_open_output);
                break;
            }
            write_to_file(output, root, separator, 1);
            printf("\nSuccessful writing to the file.\n");
            fclose(output);
            check = read_file(output, separator, &root);
            if(check == memory_error){
                for(int i = 0; i < number; i++){
                    free(words[i]);
                    words[i] = NULL;
                }
                free(words);
                return memory_error;
            }

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
        }
    }
    for(int i = 0; i < number; i++){
        free(words[i]);
        words[i] = NULL;
    }
    free(words);
    return success;
}

int write_to_file(FILE *output, Node *root, char *separator, int flag){
    if(root == NULL){
        return success;
    }

    for(int i = 0; i < root->count; i++){
        if(!flag || i > 0){
            fprintf(output, "%c", separator[0]);
        }
        fprintf(output, "%s", root->word, separator);
        flag = 0;
    }

    write_to_file(output, root->left, separator, flag);
    write_to_file(output, root->right, separator, flag);
    return success;
}

int get_tree_depth(Node *root){
    if (root == NULL){
        return 0;
    }

    int left_depth = get_tree_depth(root->left);
    int right_depth = get_tree_depth(root->right);

    if(left_depth > right_depth){
        return left_depth+1;
    }
    else{
        return right_depth+1;
    }
}

int get_long_short_word(Node *root, char **long_word, char **short_word){
    if(root == NULL){
        return success;
    }

    if(*long_word == NULL || strlen(root->word) > strlen(*long_word)){
        free(*long_word);
        *long_word = strdup(root->word);
    }
    if(*short_word == NULL || strlen(root->word) < strlen(*short_word)){
        free(*short_word);
        *short_word = strdup(root->word);
    }

    get_long_short_word(root->left, long_word, short_word);
    get_long_short_word(root->right, long_word, short_word);
}

void get_words(Node *root, Node **words, int *count, int number){
    if(root != NULL){
        for(int i = 0; i < number; i++){
            if(words[i] == NULL){
                words[i] = root;
                (*count)++;
                break;
            }
            else if(words[i]->count <= root->count){
                if(*count < number){
                    for(int j = *count; j > i; j--){
                        words[j] = words[j-1];
                    }
                    words[i] = root;
                    (*count)++;
                    break;
                }
                else{
                    for(int j = number - 1; j > i; j--){
                        words[j] = words[j-1];
                    }
                    words[i] = root;
                    break;
                }
            }
        }
        get_words(root->left, words, count, number);
        get_words(root->right, words, count, number);
    }
}


int find_word(Node *root, char *word){
    int compare;
    int count = 0;
    if(root == NULL){
        return count;
    }
    else{
        compare = strcmp(word, root->word);
        if(!compare){
            count += root->count;
        }
        else if(compare < 0){
            count += find_word(root->left, word);
        }
        else{
            count += find_word(root->right, word);
        }
    }
    return count;
}

int read_file(FILE *input, char *separator, Node **root){
    char symbol;
    int size = 10;
    char *word = (char*)malloc(size*sizeof(char));
    if(word == NULL){
        return memory_error;
    }
    int index = 0;

    while((symbol = fgetc(input)) != EOF){
        if(is_sep(symbol, separator) == fail){
            word[index] = symbol;
            index++;
            if(index == size-1){
                char *temp = (char*)realloc(word, (size*2)*sizeof(char));
                if(temp == NULL){
                    free(word);
                    return memory_error;
                }
                word = temp;
                size *= 2;
            }
        }
        else{
            if(index > 0){
                word[index] = '\0';
                *root = insert(*root, word);
                if(*root == NULL){
                    free(word);
                    return memory_error;
                }
                index = 0;
                free(word);
                word = (char*)malloc(size*sizeof(char));
                if(word == NULL){
                    return memory_error;
                }
            }
        }
    }
    if(index > 0){
        word[index] = '\0';
        *root = insert(*root, word);
        if(*root == NULL){
            free(word);
            return memory_error;
        }
    }
    free(word);
    return success;
} 

void print_tree(Node *root, int level){
    if(root != NULL){
        for(int i = 0; i < level; i++){
            if(i == level - 1){
                printf("  |--");
            }
            else{
                printf("     ");
            }
        }
        printf("%s (%d)\n", root->word, root->count);
        
        print_tree(root->left, level+1);
        print_tree(root->right, level+1);
    }
}

int is_sep(const char symbol, char *separator){
    int length = strlen(separator);
    for(int i = 0; i < length; i++){
        if(symbol == separator[i]){
            return success;
        }
    }
    return fail;
}

int total_words(Node *root){
    if(root == NULL){
        return 0;
    }
    return total_words(root->left) + total_words(root->right) + 1;
}
Node *create_node(char *word){
    Node *new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        //printf("!");
        return NULL;
    }
    new_node->word = strdup(word);
    new_node->count = 1;
    new_node->left = new_node->right = NULL;
    return new_node;
}

Node *insert(Node *root, char *word){
    int compare;
    if(root == NULL){
        return create_node(word);
    }
    else{
        compare = strcmp(word, root->word);
        if(!compare){
            root->count++;
        }
        else if(compare < 0){
            root->left = insert(root->left, word);
        }
        else{
            root->right = insert(root->right, word);
        }
    }
    return root;
}

void free_node(Node *node){
    if(node != NULL){
        free_node(node->left);
        free_node(node->right);
        free(node->word);
        free(node);
    }
}

void print(int state){
    if(state == wrong_input){
        printf("\nWrong input. Try again.\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == is_not_open_input){
        printf("File with data is not open\n");
    }
    else if(state == is_not_open_output){
        printf("Output file is not open\n");
    }
    else if(state == menu){
        printf("\n\tMAIN MENU\n 1.How many times a word is in the file\n 2.Get n of the most common words\n 3.Get the longest and the shortest word\n 4.Get depth of the tree\n 5.Print tree\n 6.Write to file\n 7.Exit\nEnter a number: ");
    }
    else if(state == after_function){
        printf("\nChoose option:\n 1. Back to menu\n 2. Exit\nEnter a number: ");
    }
}