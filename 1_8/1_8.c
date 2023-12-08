#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_amount = -1,
    wrong_flag = -2,
    not_open = -3, 
    memory_error = -4
};

long get_file_length(FILE *file){
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    return length;
}

int get_file(FILE *file, char **list, int length){
    int i = 0;
    char symbol = 0;
    while((symbol = fgetc(file)) != EOF){
        if(symbol != ',' && symbol != '.' && symbol != ' ' && symbol != '\0' && symbol != '\n' && symbol != EOF){
            (*list)[i] = symbol;
            i++;
        }
        else if((*list)[i-1] != ' ' && i < length){
            (*list)[i] = ' ';
            i++;
        }
    }
    return i;
}

int transform (char *number, int base, int k){
    int total = 0;
    int temp = 0;
    for(int i = 0; i <= k; ++i){
        if(number[i] == ' ' || number[i] == '\0' || number[i] == '\n'){
            break;
        }
        else if(isdigit(number[i])){
            temp = number[i] - '0';
            total += (temp*pow(base, k - i - 1));
        }
        else{
            temp = tolower(number[i]) - 'a' + 10;
            total += (temp*pow(base, k - i - 1));
        }
    }
    return total;
}


int function (FILE* file, FILE* out_file){
    int length = get_file_length(file);
    char *list = malloc(sizeof(char)*(length+1));
    if(list == NULL){
        return memory_error;
    }
    length = get_file(file, &list, length);
    list[length] = '\0';
    int base = 0;
    int result = 0;
    char *number = malloc(sizeof(char)*length);
    if(number == NULL){
        free(list);
        return memory_error;
    }
    int k = 0;
    int j = 0;
    for (int i = 0; i < length; ++i){
        while(list[i] != ' ' && i < length){
            if (isdigit(list[i])){
                if (list[i] - '0' + 1 > base){
                    base = list[i] - '0' + 1;
                }
                number[k] = list[i];
            }
            else{
                if (tolower(list[i]) - 'a' + 11 > base){
                    base = tolower(list[i]) - 'a' + 11;
                }
                number[k] = list[i];
            }
            k++;
            i++;
        }
        number[k] = '\0';
        result = transform(number, base, k);
        if(number[j] == '0'){
            while(number[j] == '0' && j < k){
                j++;
            }
        }
        if(strlen(number) == 1 && number[0] == '0'){
            fprintf(out_file,"%c %d %d\n", number[0], base+1, result);
        }
        if(result != 0 || number[k] == '0'){
            if(number[0] == '0'){
                for(int i = j; i < k; ++i){
                    fprintf(out_file,"%c", number[i]);
                }
            }
            else{
                for(int i = 0; i < k; ++i){
                    fprintf(out_file,"%c", number[i]);
                }
            }
            fprintf(out_file, " %d %d\n", base, result);
        }
        free(number);
        number = malloc(sizeof(char)*length);
        if(number == NULL){
            free(list);
            return memory_error;
        }
        j = 0;
        k = 0;
        base = 0;
        result = 0;
    }
    free(list);
    free(number);
    return success;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Wrong amount of arguments");
        return wrong_amount;
    }
    
    FILE *file = fopen(argv[1], "r");
    FILE *out_file = fopen(argv[2], "w");
    if(!file || !out_file){
    printf("File is not open");
        return not_open;
    }
    if(function(file, out_file)){
        printf("success");
    }
    else {printf("fail");}

    fclose(file);
    fclose(out_file);
    
    return 0;
}