#include "read_file.h"


Status read_file_with_settings(FILE *file, char **save, char **opers, Syntax *sntx){
    if(!file){
        return wrong_argument;
    }

    char *line = NULL;
    size_t size = 0;
    int flag = 0;
    while(getline(&line, &size, file) != -1){
        char *token = strtok(line, " \t\n\r");
        
        if(token[0] == '#'){
            continue;
        }
        else if(token[0] == '['){
            flag = 1;
            continue;
        }
        else if(strstr(token, "]") && flag == 1){
            flag = 0;
            continue;
        }

        if(!token){
            continue;
        }

        if(!strcmp(token, "right=") && !flag){
            *save = "right";
            continue;
        }

        if(!strcmp(token, "left=") && !flag){
            *save = "left";
            continue;
        }

        if(!strcmp(token, "op()") && !flag){
            char *token_2 = strtok(NULL, " \t\n\r");
            if(!token_2){
                continue;
            }

            for(int i = 0; i < OP_COUNT; i++){
                if(!strcmp(token_2, operations[i])){
                    sntx[i] = LEFT;
                    break;
                }
            }
            continue;
        }

        if(!strcmp(token, "()op") && !flag){
            char *token_2 = strtok(NULL, " \t\n\r");
            if(!token_2){
                continue;
            }

            for(int i = 0; i < OP_COUNT; i++){
                if(!strcmp(token_2, operations[i])){
                    sntx[i] = RIGHT;
                    break;
                }
            }
            continue;
        }

        if(!strcmp(token, "(op)") && !flag){
            char *token_2 = strtok(NULL, " \t\n\r");
            if(!token_2){
                continue;
            }

            for(int i = 0; i < OP_COUNT; i++){
                if(!strcmp(token_2, operations[i])){
                    if(types[i] == BINARY){
                        sntx[i] = MIDDLE;
                        break;
                    }
                }
            }
            continue;
        }
        if(!flag){
            for(int i = 0; i < OP_COUNT; i++){
                if(!strcmp(token, operations[i])){
                    token = strtok(NULL, " \t\n\r");
                    if(!token){
                        continue;
                    }
                    
                    opers[i] = strdup(token);
                    break;
                }
            }
        }
        char *token_2 = strtok(NULL, " \t\n\r");
        if(!token_2){
            continue;
        }
        if(strstr(token_2, "]") && flag == 1){
            flag = 0;
            continue;
        }
        continue;
    }
    free(line);
}