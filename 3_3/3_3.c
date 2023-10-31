#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    is_not_open_output = -4,
    fail = -5
};

typedef struct{
    int id;
    char *name;
    char *surname;
    double salary;
}Employee;

int check_flag(char *flag);
int check_name(char *name);
int reading(FILE *input, Employee **employees, int *number_employ);
int compare_flag_a(const void *first, const void *second);
int compare_flag_d(const void *first, const void *second);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 4){
        print(wrong_input);
        return wrong_input;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return (is_not_open_input);
    }

    if(check_flag(argv[2]) == fail){
        print(wrong_input);
        return wrong_input;
    }
    char flag = argv[2][1];

    FILE *output = fopen(argv[3], "w");
    if(!output){
        print(is_not_open_output);
        return is_not_open_output;
    }

    Employee *employees = NULL;
    int number_employ = 0;
    int result = reading(input, &employees, &number_employ);
    if(result == memory_error){
        print(memory_error);
        return memory_error;
    }

    if(flag == 'a'){
        qsort(employees, number_employ, sizeof(Employee), compare_flag_a);
    }
    else{
        qsort(employees, number_employ, sizeof(Employee), compare_flag_d);
    }
    
    for(int i = 0; i < number_employ; i++){
        fprintf(output, "%d %s %s %lf\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }

    printf("Program finished");
    
    for(int i = 0; i < number_employ; i++){
        free(employees[i].name);
        free(employees[i].surname);
    }
    free(employees);
    fclose(input);
    fclose(output);
    return success;
}

int reading(FILE *input, Employee **employees, int *number_employ){
    int count = 10;
    *employees = (Employee *)malloc(count*sizeof(Employee));
    if(*employees == NULL){
        return memory_error;
    }
    *number_employ = 0;
    char *line = NULL;
    size_t line_length = 0;
    while(getline(&line, &line_length, input) != -1){
        if(*number_employ == count){
            count *= 2;
            Employee *temp = (Employee*)realloc(temp, count*sizeof(Employee));
            if(temp == NULL){
                free(line);
                return (memory_error);
            }
            *employees = temp;
        }
        int id;
        char name[256];
        char surname[256];
        double salary;
        if(sscanf(line, "%d %255s %255s %lf", &id, name, surname, &salary) == 4){
            if(check_name(name) == fail || check_name(surname) == fail){
                continue;
            }
            if(id < 0 || salary < 0){
                continue;
            }
            Employee *cur_employ = &(*employees)[*number_employ];
            cur_employ->id = id;
            cur_employ->name = (char*)malloc((strlen(name) + 1)*sizeof(char));
            cur_employ->surname = (char*)malloc((strlen(surname) + 1)*sizeof(char));
            if(cur_employ->name == NULL || cur_employ->surname == NULL){
                free(line);
                return memory_error;
            }
            strcpy(cur_employ->name, name);
            strcpy(cur_employ->surname, surname);
            cur_employ->salary = salary;
            (*number_employ)++;
        }
    }
    free(line);
    return success;
}

int compare_flag_a(const void *first, const void *second){
    const Employee *emp_1 = (const Employee*)first;
    const Employee *emp_2 = (const Employee*)second;

    if (emp_1->salary < emp_2->salary) return -1;
    if (emp_1->salary > emp_2->salary) return 1;

    int sur_comp = strcmp(emp_1->surname, emp_2->surname);
    if(sur_comp != 0) return sur_comp;

    int name_comp = strcmp(emp_1->name, emp_2->name);
    if(name_comp != 0) return name_comp;

    return emp_1->id - emp_2->id;
}

int compare_flag_d(const void *first, const void *second){
    const Employee *emp_1 = (const Employee*)first;
    const Employee *emp_2 = (const Employee*)second;

    if (emp_1->salary < emp_2->salary) return 1;
    if (emp_1->salary > emp_2->salary) return -1;

    int sur_comp = strcmp(emp_1->surname, emp_2->surname);
    if(sur_comp != 0) return -sur_comp;

    int name_comp = strcmp(emp_1->name, emp_2->name);
    if(name_comp != 0) return -name_comp;

    return emp_2->id - emp_1->id;
}

int check_name(char *name){
    int length = strlen(name);
    for(int i = 0; i < length; i++){
        if(!isalpha(name[i])){
            return fail;
        }
    }
    return success;
}

int check_flag(char *flag){
    if(flag[0] == '-' || flag[0] == '/'){
        if(flag[1] == 'a' || flag[1] == 'd'){
            return success;
        }
    }
    return fail;
}

void print(int state){
    if(state == wrong_input){
        printf("Wrong input\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == is_not_open_input){
        printf("File with data is not open\n");
    }
    else if(state == is_not_open_output){
        printf("Output file is not open\n");
    }
}