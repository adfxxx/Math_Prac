#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>

enum errors{
    success = 1,
    wrong_amount = -1,
    wrong_flag = -2,
    not_open = -3
};

int flag_check(char *flag){
    if (flag[0] == '-' || flag[0] == '/'){
        if (flag[0] == 'r' || flag[0] == 'a'){
            return success;
        }
        else{
            return wrong_flag;
        }
    }
    else{
        return wrong_flag;
    }
}

long get_file_length(FILE *file){
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    return length;
}

void get_file(FILE *file, char **list, int length){
    int i = 0;
    char symbol;
    char *list_temp = malloc(sizeof(char)*length);
    (*list) = list_temp;
    while(!feof(file)){
        symbol = fgetc(file);
        if (symbol != ',' && symbol != '.' && symbol != EOF && symbol != '\0'){
            (*list)[i] = symbol;
            i++;
        }
        else{
            (*list)[i] = ' ';
            i++;
        }
    }
    (*list)[i] = '\0';
}

void flag_r (FILE *file_1, FILE *file_2, FILE *file_out){
    int length_1 = get_file_length(file_1);
    int length_2 = get_file_length(file_2);
    char *list_1 = NULL;
    char *list_2 = NULL;
    get_file(file_1, &list_1, length_1);
    get_file(file_2, &list_2, length_2);
    int j = 0;
    int k = 0;
    if (length_1 > length_2){
        for(int i = 0; i < length_1; ++i){
            while(list_1[j] != ' ' && list_1[j] != '\0'){
                fprintf(file_out, "%c", list_1[j]);
                j++;
            }
            printf("\n");
            while(list_2[k] != ' '  && list_2[k] != '\0'){
                if (k < length_2){
                    fprintf(file_out, "%c", list_2[k]);
                    k++;
                }
            }
            k++;
            j++;
        }    
    }
    else{
       for(int i = 0; i < length_2; ++i){
            while(list_1[j] != ' ' && list_1[j] != '\0'){
                if (j < length_1){
                    fprintf(file_out, "%s", list_1[j]);
                    j++;
                }
            }
            while(list_2[k] != ' ' && list_2[k] != '\0'){
                fprintf(file_out, "%s", list_2[k]);
                k++;
            }
            k++;
            j++;
        }    
    }
}

int main(int argc, char *argv[]){
    if (argc > 5 || argc < 4){
        printf("Wrong amount of arguments");
        return wrong_amount;
    }
    if(!flag_check(argv[1])){
        printf("Wrong flag");
        return wrong_flag;
    }
    char flag = argv[1][1];
    switch(flag){
        case 'r':
            if(argc != 5){
                printf("Wrong amount of arguments");
                return wrong_amount;
            }
            FILE *file_1 = fopen(argv[2], "r");
            FILE *file_2 = fopen(argv[3], "r");
            FILE *file_out = fopen(argv[4], "w");
            if(!file_1 || !file_2 || !file_out){
                printf("File is not open");
                return not_open;
            }
            flag_r(file_1, file_2, file_out);
            printf("Success");
            break;

        // case 'a':
        //     if(argc != 4){
        //         printf("Wrong amount of arguments");
        //         return wrong_amount;
        //     }
        //     FILE *file = fopen(argv[2], "r");
        //     FILE *file_out = fopen(argv[3], "w");
        //     if(!file|| !file_out){
        //         printf("File is not open");
        //         return not_open;
        //     }
        //     flag_a(file, file_out);
        //     printf("Success");
        //     break;

    }
    return 0;
}