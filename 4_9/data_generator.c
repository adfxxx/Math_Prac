#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions/status.h"

char *generate_text();

int main(){
    srand(time(NULL));
    FILE *file = fopen("data.txt", "w");

    int data_count = rand() % 10 + 1;
    for(int i = 0; i < data_count; i++){
        int year = rand() % (2024 - 2000 + 1) + 2000;
        int month = rand() % 12 + 1;
        int day = rand() % 31 + 1;
        int hour = rand() % (22 + 1) + 1;
        int minute = rand() % (59 + 1) + 1;
        int second = rand() % (59 + 1) + 1;
        fprintf(file, "%04d-%02d-%02dT%02d:%02d:%02d ", year, month, day, hour, minute, second);

        int priority = rand() % 10 + 1;
        fprintf(file, "%d ", priority);

        int dep_id = rand() % 100 + 1;
        fprintf(file, "%d ", dep_id);

        int id = rand() % 100 + 1;
        fprintf(file, "%d ", id);

        char *str = generate_text();
        if(!str){
            fclose(file);
            print_error(error_allocation);
            return error_allocation;
        }
        fprintf(file, "%s\n", str);
        free(str);
    }
    printf("Success\n");
    fclose(file);
    return success;
}

char *generate_text(){
    char symbols[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789";
    int len = sizeof(symbols) - 1;
    int total_len = rand() % 100 + 1;
    char *text = (char*)malloc(sizeof(char) * (total_len + 1));
    if(!text){
        return NULL;
    }
    for(int i = 0; i < total_len; i++){
        text[i] = symbols[rand() % len];
    }
    text[total_len] = '\0';
    return text;
}
