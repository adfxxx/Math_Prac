#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int flag_check(char *flag){
    char flags_1 [8][3] = {"-d", "/d", "-i", "/i", "-s", "/s", "-a", "/a"};
    char flags_2 [8][4] = {"-nd", "/nd", "-ni", "/ni", "-ns", "/ns", "-na", "/na"};

    if (2 < strlen(flag) && strlen(flag) > 3 ){
        return 0;
    }
    if (strlen(flag) == 2){
        for (int i = 0; i < 8; ++i){
            if (strcmp(flag, flags_1[i]) == 0){
                return 1;
            }
        }
    }
    if (strlen(flag) == 3){
        for (int i = 0; i < 8; ++i){
            if (strcmp(flag, flags_2[i]) == 0){
                return 2;
            }
        }
    }
    return 0;
}

int output_name (char *input, int length, char *end, int length_end, char **output){
    int position = 0;
    char *end_input;
    for (int i = length; i > 0 ; --i){
        if (input[i] == '\\'){
            position = i + 1;
            break;
        }
    }
    *output = (char*)malloc(sizeof(char)*(length_end + length + 1));
    if(*output == NULL){
        return 0;
    }
    if (position == 0){
        memcpy(*output, end, length_end);
        memcpy(*output + length_end, input, length+1);
    }
    else{
        end_input = (char*)malloc(sizeof(char)*(length-position));
        if(end_input == NULL){
            free(*output);
            return 0;
        }
        int k = 0;
        for (int i = position; i <= length; ++i){
            end_input[k] = input[i];
            k++;
        }
        memcpy(*output, input, position);
        memcpy(*output + position, end, length_end);
        memcpy(*output + length_end + position, end_input, length-position+2);
        free(end_input);
    }
    return 1;
}

void flag_d (FILE *input, FILE *output){
    char symbol;
    while (!feof(input)){
        symbol = fgetc(input);
        if (!(symbol >= '0' && symbol <= '9') && symbol != EOF){
            fputc(symbol, output);
        }
    }
}

void flag_i (FILE *input, FILE *output){
    char symbol;
    int count = 0;
    while (!feof(input)){
        symbol = fgetc(input);
        if (symbol != '\n' && symbol != EOF){
            if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z')){
                count += 1;
            }
        }
        else{
            fputs ("Number ", output);
            fprintf(output, "%d\n", count);
            count = 0;
        }
    }
}

void flag_s (FILE *input, FILE *output){
    char symbol;
    int count = 0;
    while (!feof(input)){
        symbol = fgetc(input);
        if (symbol != '\n' && symbol != EOF){
            if (!((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'))){
                if (!(symbol >= '0' && symbol <= '9') && symbol != ' '){
                    count += 1;
                }
            }
        }
        else{
            fprintf(output, "Number: %d\n", count);
            count = 0;
        }
    }
}

void flag_a(FILE *input, FILE *output){
    char symbol;
    int ascii;
    while (!feof(input)){
        symbol = fgetc(input);
        if (symbol != EOF){
            if (!(symbol >= '0' && symbol <= '9') && symbol != '\n'){
                fprintf(output, "%X", symbol);
            }
            else{
                fputc(symbol, output);
            }
        }
    }
}

int main (int argc, char *argv[]){
    char flag;
    char *output = NULL;
    if (argc > 4 || argc < 3){
        printf ("Input must be like: nflag input_file or flag input_file output_file");
        return 0;
    }
    char *input = argv[2];
    if (argc == 4){
        if (flag_check(argv[1]) == 1){
            output = argv[3];
            flag = argv[1][1];
        }
        else {
            printf ("Wrong flag");
            return 0;
        }
    }
    if (argc == 3){
        if (flag_check(argv[1]) == 2){
            char *end= "out_";
            int length_end = strlen(end);
            int length = strlen(input);
            if(output_name(input, length, end, length_end, &output) == 0){
                printf("Memory error.\n");
                return 0;
            }
            flag = argv[1][2];
        }
        else{
            printf ("Wrong flag");
            return 0;
        }
    }

    FILE *input_file = fopen(input, "r");
    if (!input_file){
        printf ("Input_file is not open");
        return 0;
    }
    FILE *output_file = fopen(output, "w");
    if (!output_file){
        printf ("Output_file is not open");
        fclose(input_file);
        return 0;
    }

    switch (flag){
        case 'd':
            flag_d(input_file, output_file);
            printf("Sucessful output");
            break;
        case 'i':
            flag_i(input_file, output_file);
            printf("Sucessful output");
            break;
        case 's':
            flag_s(input_file, output_file);
            printf("Sucessful output");
            break;
        case 'a':
            flag_a(input_file, output_file);
            printf("Sucessful output");
            break;
    }
    free(output);
    fclose(input_file);
    fclose(output_file);
    return 0;
}