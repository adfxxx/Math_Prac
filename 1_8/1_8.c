#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_amount = -1,
    wrong_flag = -2,
    not_open = -3
};

long get_file_length(FILE *file){
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    return length;
}

void get_file(FILE *file, char *list, int length){
    int i = 0;
    char symbol = 0;
    while((symbol = fgetc(file)) != EOF){
        if (symbol != ',' && symbol != '.' && symbol != ' ' && symbol != '\0' && symbol != '\n'){
            list[i] = symbol;
            i++;
        }
        else if (list[i-1] != ' ' && i < length){
            list[i] = ' ';
            i++;
        }
    }
    list[i] = '\0';
}

int transform (int length, char *number, int base, int k){
    int total = 0;
    int temp = 0;
    for(int i = k; i < length; ++i){
        if(number[i] == ' ' || number[i] == '\0' || number[i] == '\n'){
            break;
        }
        else if(isdigit(number[i])){
            temp = number[i] - '0';
            total += (temp*pow(base, length - i - 1));
        }
        else{
            temp = tolower(number[i]) - 'a' + 10;
            total += (temp*pow(base, length - i - 1));
        }
    }
    return total;
}


int function (FILE* file, FILE* out_file){
    int length = get_file_length(file);
    char *list = malloc(length);
    get_file(file, list, length);
    int base = 0;
    int count = 0;
    int result = 0;
    char *number = malloc(length);
    int k = 0;
    if(!isdigit(list[0]) && !isalpha(list[0])){
        return success;
    }
    for (int i = 0; i < length; ++i){
        while(list[i] != ' ' && i < length){
            if (isdigit(list[i])){
                if (list[i] - '0' + 1 > base){
                     base = list[i] - '0' + 1;
                }
                number[i] = list[i];
            }
            else{
                if (tolower(list[i]) - 'a' + 11 > base){
                     base = tolower(list[i]) - 'a' + 11;
                }
                number[i] = list[i];
            }
            count += 1;
            i++;
        }
        result = transform(count, number, base, k);
        if (result != 0){
            for(int i = k; i < count; ++i){
                fprintf(out_file,"%c", number[i]);
            }
            fprintf(out_file, " %d %d\n", base, result);
        }
        k = count+1;
        count++;
        base = 0;
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