#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

enum{
    success = 1,
    is_not_open = -1,
    fail = -2
};

int get_file(char *str, ...);
void find_sub(FILE *file, char *str, char *file_name);
void print(char *file_name, int number, int position);

int main(){
    char *str = "7.9";
    char *file1 = "file1.txt";
    char *file2 = "file2.txt";
    char *file3 = "file3.txt";
    char *end = "-1";
    if(get_file(str, file1, file2, file3, end) == is_not_open){
        printf("File is not open");
        return is_not_open;
    }
    return 0;
}

int get_file(char *str, ...){
    va_list files;
    va_start(files, str);
    char *file_name = va_arg(files, char*);
    while(file_name != "-1"){
        FILE *file = fopen(file_name, "r");
        if(file == NULL){
            return is_not_open;
        }
        find_sub(file, str, file_name);
        fclose(file);
        file_name = va_arg(files, char*);
    }
    va_end(files);
}

void print(char *file_name, int number, int position){
     printf("File %s: line - %d, position - %d\n", file_name, number, position);
}

void find_sub(FILE *file, char *str, char *file_name){
    char *line = NULL;
    int length = 0;
    int number = 0;
    int str_length = strlen(str);
    while(getline(&line, &length, file) != -1){
        number++;
        int position = 0;
        int line_length = strlen(line);
        int j = 0;
        for (int i = 0; i < line_length; i++){
            for(j = 0; j < str_length; j++){
                if(line[i+j] != str[j]){
                    break;
                }
            }
            if(j == str_length){
                position = i + 1;
                print(file_name, number, position);
                position++;
            }
        }
    }
    if(line){
        free(line);
    }
}