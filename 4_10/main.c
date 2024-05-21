#include "read_file.h"

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "r");
    if(!file){
        print_error(input_file_is_not_open);
        return input_file_is_not_open;
    }
    char *save = "left";
    read_file_with_settings(file, &save, operations, syntax);

    for(int i = 0; i < 13; i++){
        printf("%s\n", operations[i]);
    }
}