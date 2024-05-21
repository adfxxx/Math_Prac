#include "status.h"

void print_error(Status status){
    switch(status){
        case error_allocation:
            printf("ERROR DURING MEMORY ALLOCATION.\n");
            break;
        case wrong_argument:
            printf("WRONG ARGUMENT IN FUNCTION.\n");
            break;
        case data_not_found:
            printf("DATA WAS NOT FOUND.\n");
            break;
        case input_file_is_not_open:
            printf("INPUT FILE IS NOT OPEN.\n");
            break;
        case wrong_priority:
            printf("WRONG INPUT PRIORITY.\n");
            break;
        case wrong_file:
            printf("ERROR DURING READING THE FILE.\n");
            break;
        case wrong_data:
            printf("WRONG DATA IN THE FILE.\n");
            break;
        case already_exist:
            printf("THIS DATA ALREADY EXISTS.\n");
            break;
        case wrong_input:
            printf("WRONG INPUT.\n");
            break;
        case log_file_is_not_open:
            printf("FILE FOR LOGS IS NOT OPEN.\n");
            break;
        case error:
            printf("SOME ERROR DURING THE FUNCTION.\n");
            break;
    }
}
