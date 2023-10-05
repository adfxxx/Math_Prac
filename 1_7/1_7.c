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
    char symbol = 0;
    char *list_temp = malloc(sizeof(char)*length);
    (*list) = list_temp;
    while((symbol = fgetc(file)) != EOF){
        if (symbol != ',' && symbol != '.' && symbol != ' ' && symbol != '\0' && symbol != '\n'){
            (*list)[i] = symbol;
            i++;
        }
        else if ((*list)[i-1] != ' ' && i < length){
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
            while(list_1[j] != ' ' && j < length_1){
                fprintf(file_out, "%c", list_1[j]);
                j++;
            }
            j++;
            fprintf(file_out, " ");
            while(list_2[k] != ' ' && k < length_2){
                fprintf(file_out, "%c", list_2[k]);
                k++;
            }
            k++;
            if (k < length_2){
                fprintf(file_out, " ");
            }
            if (j > length_1){
                break;
            }
        }    
        free(list_1);
        free(list_2);
    }
    else{
       for(int i = 0; i < length_2; ++i){
            while(list_1[j] != ' ' && j < length_1){
                fprintf(file_out, "%c", list_1[j]);
                j++;
            }
            j++;
            fprintf(file_out, " ");
            while(list_2[k] != ' ' && k < length_2){
                fprintf(file_out, "%c", list_2[k]);
                k++;
            }
            k++;
            if (j < length_1){
                fprintf(file_out, " ");
            }
            if (k > length_2){
                break;
            }
        }
        free(list_1);
        free(list_2);  
    }
}

int to_base_4(int number){
    int answer = 0;
    while (number != 0){
        answer = answer + (number % 4);
        number /= 4;
        answer *= 10;
    }
    int total = 0;
    while (answer != 0){
        total = total*10 + (answer % 10);
        answer /= 10;
    }
    return total;
}

void flag_a (FILE* file, FILE* file_out){
    int length = get_file_length(file);
    char *list = NULL;
    get_file(file, &list, length);
    int count = 1;
    char symbol = 0;
    for (int i = 0; i < length; ++i){
        if (count % 10 == 0){
            while (list[i] != ' ' && i < length){
                symbol = tolower(list[i]);
                int ascii = symbol;
                fprintf(file_out, "%d", to_base_4(ascii));
                i++;
            }
            fputc(' ', file_out);
        }
        if (count % 2 == 0 && count % 10 != 0){
            while (list[i] != ' ' && i < length){
                fprintf(file_out, "%c", tolower(list[i]));
                i++;
            }
            fputc(' ', file_out);
        }
        if (count % 5 == 0 && count % 10 != 0){
            while (list[i] != ' ' && i < length){
                int ascii = list[i];
                fprintf(file_out, "%o", ascii);
                i++;
            }
            fputc(' ', file_out);
        }
        if (list[i] == ' ' && list[i-1] != ' '){
            count++;
            fputc(' ', file_out);
        }
        else if (i < length){
            fprintf(file_out, "%c", list[i]);
        }
    }
    free(list);
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
            fclose(file_1);
            fclose (file_2);
            fclose (file_out);
            break;

        case 'a':
            if(argc != 4){
                printf("Wrong amount of arguments");
                return wrong_amount;
            }
            FILE *file = fopen(argv[2], "r");
            FILE *out_file = fopen(argv[3], "w");
            if(!file|| !out_file){
                printf("File is not open");
                return not_open;
            }
            flag_a(file, out_file);
            printf("Success");
            fclose (file);
            fclose (out_file);
            break;
    }
    return 0;
}