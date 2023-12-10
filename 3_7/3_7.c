#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

enum errors{
    success = 1,
    memory_error = -1,
    fail = -2,
    wrong_option = -3,
    wrong_input = -4,
    eps_error = -5,
    not_found = -6,
    empty_list = -7
};

enum states{
    menu = 1,
    after_function = 2,
    same = 3
};

typedef struct Liver{
    char *name;
    char *surname;
    char *patronymic;
    int day;
    int month;
    int year;
    char gender;
    double salary;
    struct Liver *next;
}Liver;

typedef struct Changes{
    int option;
    int operation;
    Liver *old_state;
    char *old_value;
    double old_salary;
    int old_date;
    char old_gen;
    struct Changes *next;
}Changes;

typedef struct{
    Liver *head;
    Changes *change_history;
}List;

void change_back(Changes *change, List *list, Liver *liver);
int check_identity(Liver *liver_1, Liver *liver_2);
void undo_change(List *list, Changes *change);
void undo(List *list, int number);
void remove_liver(List *list, Liver *old_liver);
Liver *copy_liver(const Liver *liver);
int change_liver(Liver *liver, int option, List *list);
int add_changing(List *list, Liver *old_liver, Liver *new_liver, int operation, int option);
void free_changes(Changes *change);
void write_to_file(const List *list, FILE *output);
int find_liver(const List *list, const char *name, const char *surname, const char *patronymic, int day, int month, int year, char gender, double salary, double eps, Liver **found);
void print_list(const List *list);
int read_file(FILE *input, List *list);
void add_liver_to_list(List *list, Liver *new_liver);
Liver *create_liver(const char *name, const char *surname, const char *patronymic, int day, int month, int year, char gender, double salary);
int check_name(const char *name);
void free_liver(Liver *liver);
void free_list(List *list);
void print(int state);

int main(){
    List list = {NULL};
    int option = 0;
    char *line = NULL;
    size_t len = 0;
    FILE *input;
    FILE *output;
    int number;
    double eps = 0.000001;
    int n = 0;
    while (option != 9){
        char name[256];
        char surname[256];
        char patronymic[256];
        int day;
        int month;
        int year;
        char gender;
        double salary;
        int result = 0;
        Liver *res;
        Liver *temp;
        int check;
        option = 0;
        fflush(stdin);
        print(menu);
        if(!scanf("%d", &option) || option < 1 || option > 9){
            print(wrong_option);
            print(menu);
            while(!scanf("%d", &option) || option < 1 || option > 9){
                fflush(stdin);
                print(wrong_option);
                print(menu);
            }
        }
        switch (option){
            case 1:
            fflush(stdin);
            printf("\nEnter name of file: ");
            result = getline(&line, &len, stdin);
            if(result != -1){
                if(line[result - 1] == '\n'){
                    line[result - 1] = '\0';
                }
            }
            input = fopen(line, "r");
            if(!input){
                print(wrong_input);
                free(line);
                break;
            }
            result = read_file(input, &list);
            if(result == memory_error){
                print(memory_error);
                free(line);
                fclose(input);
                break;
            }
            else{
                printf("\nSuccessful writing.\n");
                free(line);
                fclose(input);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if(option == 2){
                option = 9;
            }
            break;
            case 2:
            if(list.head == NULL){
                printf("\nThe list is empty now.\n");
                break;
            }
            fflush(stdin);
            printf("\nEnter a liver to find: name surname patronymic(or -) day month year gender salary.\nEnter: ");
            if(fscanf(stdin, "%255s %255s %255s %d %d %d %c %lf", name, surname, patronymic, &day, &month, &year, &gender, &salary) != 8){
                print(wrong_input);
            }
            else{
                result = find_liver(&list, name, surname, patronymic, day, month, year, gender, salary, eps, &temp);
                if(result == eps_error){
                    printf("Wrong eps.\n");
                }
                else if(result == not_found){
                    printf("\nThe liver is not in the system.\n");
                }
                else{
                    printf("\nThe liver is found in the system.\n");
                }
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
            case 3:
            if(list.head == NULL){
                printf("\nThe list is empty now.\n");
                break;
            }
            fflush(stdin);
            printf("\nEnter a liver to change: name surname patronymic(or -) day month year gender salary.\nEnter: ");
            if(fscanf(stdin, "%255s %255s %255s %d %d %d %c %lf", name, surname, patronymic, &day, &month, &year, &gender, &salary) != 8){
                print(wrong_input);
                break;
            }
            else{
                result = find_liver(&list, name, surname, patronymic, day, month, year, gender, salary, eps, &temp);
                if(result == eps_error){
                    printf("Wrong eps.\n");
                }
                else if(result == not_found){
                    printf("\nThe liver is not in the system.\n");
                }
                else{
                    fflush(stdin);
                    printf("\nChoose what do you want to change:\n 1.Name\n 2.Surname\n 3.Patronomyc\n 4.Day of birth\n 5.Month of birth\n 6.Year of birth\n 7.Gender\n 8.Salary\nEnter a number: ");
                    if(!scanf("%d", &check) || check < 1 || check > 8){
                        print(wrong_input);
                        break;
                    }
                    printf("\nEnter new information: ");
                    result = change_liver(temp, check, &list);
                    if(result == memory_error){
                        print(memory_error);
                        break;
                    }
                    else if(result == same){
                        print(same);
                    }
                    else{
                        printf("\nSuccessful changing.\n");
                        number++;
                    }
                }
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
            case 4:
            fflush(stdin);
            printf("\nEnter a liver to add: name surname patronymic(or -) day month year gender salary.\nEnter: ");
            if(fscanf(stdin, "%255s %255s %255s %d %d %d %c %lf", name, surname, patronymic, &day, &month, &year, &gender, &salary) != 8){
                print(wrong_input);
                break;
            }
            else{
                result = find_liver(&list, name, surname, patronymic, day, month, year, gender, salary, eps, &temp);
                if(result == eps_error){
                    printf("Wrong eps.\n");
                }
                else if(result == success){
                    printf("Liver is in the system already.\n");
                }
                else{
                    res = create_liver(name, surname, patronymic, day, month, year, gender, salary);
                    if(res == NULL){
                        print(wrong_input);
                    }
                    else{
                        add_changing(&list, res, NULL, 3, 0);
                        add_liver_to_list(&list, res);
                        printf("\nSuccessful adding.\n");
                        number++;
                    }
                }
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
            case 5:
            if(list.head == NULL){
                printf("\nThe list is empty now.\n");
                break;
            }
            fflush(stdin);
            printf("\nEnter a liver to delete: name surname patronymic(or -) day month year gender salary.\nEnter: ");
            if(fscanf(stdin, "%255s %255s %255s %d %d %d %c %lf", name, surname, patronymic, &day, &month, &year, &gender, &salary) != 8){
                print(wrong_input);
                break;
            }
            else{
                result = find_liver(&list, name, surname, patronymic, day, month, year, gender, salary, eps, &temp);
                if(result == eps_error){
                    printf("Wrong eps.\n");
                }
                else if(result == not_found){
                    printf("\nThe liver is not in the system.\n");
                }
                else{
                    add_changing(&list, temp, NULL, 2, 0);
                    remove_liver(&list, temp);
                    printf("\nSuccessful removing.\n");
                    number++;
                }
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
            case 6:
            if(list.head == NULL){
                printf("There are no livers in the system.\n");
            }
            else{
                print_list(&list);
            }

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
            case 7:
            fflush(stdin);
            printf("\nEnter name of file: ");
            result = getline(&line, &len, stdin);
            if(result != -1){
                if(line[result - 1] == '\n'){
                    line[result - 1] = '\0';
                }
            }
            output = fopen(line, "w");
            if(!output){
                print(wrong_input);
                free(line);
                break;
            }
            write_to_file(&list, output);
            printf("\nSuccessful writing.\n");
            fclose(output);
            free(line);
            
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
            case 8:
            printf("\nEnter n: ");
            if(!scanf("%d", &n)){
                print(wrong_input);
                break;
            }
            if(n/2 > number){
                printf("Not enough operations to undo.\n");
            }
            else{
                undo(&list, n/2);
                number = 0;
            }

            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_option);
                print(after_function);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                        fflush(stdin);
                        print(wrong_option);
                        print(after_function);
                }
            }
            if (option == 2){
                option = 9;
            }
            break;
        }
    }
    free_list(&list);
    printf("Goodbye!");
    return success;
}

void undo(List *list, int n){
    Changes *cur = list->change_history;
    Changes *prev = NULL;
    while(cur != NULL && n > 0){
        n--;
        undo_change(list, cur);
        cur = cur->next;
    }
    list->change_history = cur;
}

void undo_change(List *list, Changes *change){
    if(list == NULL || change == NULL){
        return;
    }
    Liver *new_liver;
    switch ((change->operation)){
        case 1:
        new_liver = copy_liver(change->old_state);
        change_back(change, list, new_liver);
        remove_liver(list, change->old_state);
        add_liver_to_list(list, new_liver);
        break;
        case 2:
        new_liver = copy_liver(change->old_state);
        add_liver_to_list(list, new_liver);
        break;
        case 3:
        remove_liver(list, change->old_state);
        break;
    }
    free_changes(change);
}

void change_back(Changes *change, List *list, Liver *liver){
    switch(change->option){
        case 1:
        free(liver->name);
        liver->name = strdup(change->old_value);
        break;
        case 2:
        free(liver->surname);
        liver->surname = strdup(change->old_value);
        break;
        case 3:
        free(liver->patronymic);
        liver->patronymic = strdup(change->old_value);
        break;

        case 4:
        liver->day = change->old_date;
        break;
        case 5:
        liver->month = change->old_date;
        break;
        case 6:
        liver->year = change->old_date;
        break;

        case 7:
        liver->gender = change->old_gen;
        break;
        case 8:
        liver->salary = change->old_salary;
        break;
    }
}

int change_liver(Liver *old_liver, int option, List *list){
    Liver *temp = copy_liver(old_liver);
    double eps = 0.00000001;
    char new_name[256];
    char new_surname[256];
    char new_patronymic[256];
    int new_day;
    int new_month;
    int new_year;
    char new_gender;
    double new_salary;
    switch(option){
        case 1:
        if(!scanf("%255s", new_name) || check_name(new_name) != success){
            return wrong_input;
        }
        if(!strcmp(new_name, old_liver->name)){
            return same;
        }
        free(old_liver->name);
        old_liver->name = strdup(new_name);
        add_changing(list, temp, old_liver, 1, 1);
        break;
        
        case 2:
        if(!scanf("%255s", new_surname) || check_name(new_surname) != success){
            return wrong_input;
        }
        if(!strcmp(new_surname, old_liver->surname)){
            return same;
        }
        free(old_liver->surname);
        old_liver->surname = strdup(new_surname);
        add_changing(list, temp, old_liver, 1, 2);
        break;

        case 3:
        if(!scanf("%255s", new_patronymic) || check_name(new_patronymic) != success){
            return wrong_input;
        }
        if(!strcmp(new_patronymic, old_liver->patronymic)){
            return same;
        }
        free(old_liver->patronymic);
        old_liver->patronymic = strdup(new_patronymic);
        add_changing(list, temp, old_liver, 1, 3);
        break;

        case 4:
        if(!scanf("%d", &new_day) || new_day < 1 || new_day > 31){
            return wrong_input;
        }
        if(new_day == old_liver->day){
            return same;
        }
        old_liver->day = new_day;
        add_changing(list, temp, old_liver, 1, 4);
        break;

        case 5:
        if(!scanf("%d", &new_month) || new_month < 1 || new_month > 12){
            return wrong_input;
        }
        if(new_month == old_liver->month){
            return same;
        };
        old_liver->month = new_month;
        add_changing(list, temp, old_liver, 1, 5);
        break;

        case 6:
        if(!scanf("%d", &new_year) || new_year < 0){
            return wrong_input;
        }
        if(new_year == old_liver->year){
            return same;
        }
        old_liver->year = new_year;
        add_changing(list, temp, old_liver, 1, 6);
        break;

        case 7:
        if(!scanf("%c", new_gender) || (new_gender != 'W' && new_gender != 'M')){
            return wrong_input;
        }
        if(new_gender == old_liver->gender){
            return same;
        }
        old_liver->gender = new_gender;
        add_changing(list, temp, old_liver, 1, 7);
        break;

        case 8:
        if(!scanf("%lf", &new_salary) || new_salary < eps){
            return wrong_input;
        }
        if(new_salary == old_liver->salary){
            return same;
        }
        old_liver->salary = new_salary;
        add_changing(list, temp, old_liver, 1, 8);
        break;
    }
    return success;
}

void remove_liver(List *list, Liver *liver){
    Liver *prev = NULL;
    Liver *cur = list->head;
    
    while (cur != NULL && check_identity(cur, liver) != success){
        prev = cur;
        cur = cur->next;
    }
    if (cur != NULL){
        if (prev == NULL){
            list->head = cur->next;
            prev = list->head;
        } 
        else{
            prev->next = cur->next;
        }
        if(list->head == NULL){
            list->head = NULL;
        }
        free_liver(liver);
    }
}

int check_identity(Liver *liver_1, Liver *liver_2){
    if(!strcmp(liver_1->name, liver_2->name) && !strcmp(liver_1->surname, liver_2->surname) && !strcmp(liver_1->patronymic, liver_2->patronymic)){
        if(liver_1->day == liver_2->day && liver_1->month == liver_2->month && liver_1->year == liver_2->year){
            if(liver_1->gender == liver_2->gender && liver_1->salary == liver_2->salary){
                return success;
            }
        }
    }
    return fail;
}

Liver *copy_liver(const Liver *liver){
    Liver *copy = (Liver *)malloc(sizeof(Liver));
    if(copy == NULL){
        return NULL;
    }
    
    copy->name = strdup(liver->name);
    copy->surname = strdup(liver->surname);
    copy->patronymic = strdup(liver->patronymic);
    copy->day = liver->day;
    copy->month = liver->month;
    copy->year = liver->year;
    copy->gender = liver->gender;
    copy->salary = liver->salary;
    copy->next = NULL;

    return copy;
}

int add_changing(List *list, Liver *old_liver, Liver *new_liver, int operation, int option){
    Changes *change = (Changes*)malloc(sizeof(Changes));
    if(change == NULL){
        return memory_error;
    }
    if(new_liver == NULL){
        change->old_state = copy_liver(old_liver);
        if(change->old_state == NULL){
            free(change);
            return memory_error;
        }
    }
    else{
        change->old_state = copy_liver(new_liver);
        if(change->old_state == NULL){
            free(change);
            return memory_error;
        }
    }
    if(option == 1){
        change->old_value = strdup(old_liver->name);
    }
    else if(option == 2){
        change->old_value = strdup(old_liver->surname);
    }
    else if(option == 3){
        change->old_value = strdup(old_liver->patronymic);
    }
    else if(option == 4){
        change->old_date = old_liver->day;
    }
    else if(option == 5){
        change->old_date = old_liver->month;
    }
    else if(option == 6){
        change->old_date = old_liver->year;
    }
    else if(option == 7){
        change->old_gen = old_liver->gender;
    }
    else if(option == 8){
        change->old_salary = old_liver->salary;
    }
    change->option = option;
    change->operation = operation;
    change->next = list->change_history;
    list->change_history = change;
    return success;
}

void free_changes(Changes *change){
    if(change == NULL){
        return;
    }
    free_liver(change->old_state);
    free(change);
}

void write_to_file(const List *list, FILE *output){
    Liver *cur = list->head;
    while(cur != NULL){
        fprintf(output, "%s %s %s %d %d %d %c %lf\n", 
        cur->name, cur->surname, cur->patronymic, cur->day, cur->month, cur->year, cur->gender, cur->salary);
        cur = cur->next;
    }
}

int find_liver(const List *list, const char *name, const char *surname, const char *patronymic, int day, int month, int year, char gender, double salary, double eps, Liver **found){
    if(list->head == NULL){
        return empty_list;
    }
    Liver *cur = list->head;
    if(eps <= 0){
        return eps_error;
    }
    while(cur != NULL){
        if(!strcmp(cur->name, name) && !strcmp(cur->surname, surname) && !strcmp(cur->patronymic, patronymic) && cur->day == day && cur->month == month && cur->year == year && cur->gender == gender && fabs(cur->salary - salary) < eps){
            (*found) = cur;
            return success;
        }
        cur = cur->next;
    }
    return not_found;
}

void print_list(const List *list){
    Liver *cur = list->head;

    while(cur != NULL){
        printf("Information about liver:\n");
        printf("\tName: %s, surname: %s, patronymic: %s, date of birth: %d.%d.%d, gender: %c, salary: %.2f\n",
        cur->name, cur->surname, cur->patronymic, cur->day, cur->month, cur->year, cur->gender, cur->salary);
        cur = cur->next;
    }
}

int read_file(FILE *input, List *list){
    char *line = NULL;
    size_t line_length = 0;
    while(getline(&line, &line_length, input) != -1){
        char name[256];
        char surname[256];
        char patronymic[256];
        int day;
        int month;
        int year;
        char gender;
        double salary;
        int count = sscanf(line, "%255s %255s %255s %d %d %d %c %lf", name, surname, patronymic, &day, &month, &year, &gender, &salary);
        if(count == 8){
            Liver *new_liver = create_liver(name, surname, patronymic, day, month, year, gender, salary);
            if(new_liver == NULL){
                free(line);
                return wrong_input;
            } 
            add_liver_to_list(list, new_liver);
        }
    }
    free(line);
    return success;
}

void add_liver_to_list(List *list, Liver *new_liver){
    Liver *cur = list->head;
    Liver *prev = NULL;

    while(cur != NULL && cur->year <= new_liver->year){
        prev = cur;
        cur = cur->next;
    }

    if(prev == NULL){
        new_liver->next = list->head;
        list->head = new_liver;
    }
    else{
        prev->next = new_liver;
        new_liver->next = cur;
    }
}

Liver *create_liver(const char *name, const char *surname, const char *patronymic, int day, int month, int year, char gender, double salary){
    if(check_name(name) == fail|| check_name(surname) == fail){
        return NULL;
    }
    if(check_name(patronymic) == fail){
        if(strcmp(patronymic, "-") != 0){
            return NULL;
        }
    }
    if(day < 1 || day > 31 || month < 1 || month > 12 || year < 0){
        return NULL;
    }
    if(month == 2 && day > 29){
        return NULL;
    }
    if(gender != 'M' && gender != 'W'){
        return NULL;
    }
    Liver *new_liver = (Liver*)malloc(sizeof(Liver));
    if(new_liver == NULL){
        return NULL;
    }
    new_liver->name = strdup(name);
    if(new_liver->name == NULL){
        free(new_liver);
        return NULL;
    }

    new_liver->surname = strdup(surname);
    if(new_liver->surname == NULL){
        free(new_liver);
        free(new_liver->name);
        return NULL;
    }
    new_liver->patronymic = strdup(patronymic);
    if(new_liver->patronymic == NULL){
        free(new_liver);
        free(new_liver->name);
        free(new_liver->surname);
        return NULL;
    }
    new_liver->day = day;
    new_liver->month = month;
    new_liver->year = year;
    new_liver->gender = gender;
    new_liver->salary = salary;
    new_liver->next = NULL;

    return new_liver;
}

int check_name(const char *name){
    int length = strlen(name);
    for(int i = 0; i < length; i++){
        if(!isalpha(name[i])){
            return fail;
        }
    }
    return success;
}

void free_liver(Liver *liver){
    if(liver == NULL){
        return;
    }
    if(liver->name != NULL){
        free(liver->name);
        liver->name = NULL;
    }
    if(liver->surname != NULL){
        free(liver->surname);
        liver->surname = NULL;
    }
    if(liver->patronymic != NULL){
        free(liver->patronymic);
        liver->patronymic = NULL;
    }
    liver = NULL;
    return;
}

void free_list(List *list){
    if(list == NULL){
        return;
    }
    Liver *cur = list->head;
    Liver *next;

    while(cur != NULL){
        next = cur->next;
        free(cur->name);
        cur->name = NULL;
        free(cur->surname);
        cur->surname = NULL;
        free(cur->patronymic);
        cur->patronymic = NULL;
        free(cur);
        cur = next;
    }
    Changes *cur_change = list->change_history;
    Changes *next_change;
    while(cur_change != NULL){
        next_change = cur_change->next;
        free(cur_change->old_value);
        cur_change->old_value = NULL;
        cur_change = next_change;
    }
    list->head = NULL;
    list->change_history = NULL;
}

void print(int state){
    if(state == menu){
        printf("\tMAIN MENU\nChoose option:\n 1. Add livers from file\n 2. Find liver\n 3. Change information about liver\n 4. Add liver\n 5. Delete liver\n 6. Print list\n 7. Print to the file\n 8. Undo n/2 operations\n 9. Exit\nEnter a number: ");
    }
    else if(state == wrong_option){
        printf("Wrong option. Try again.\n");
    }
    else if(state == wrong_input){
        printf("Wrong input. Try again.\n");
    }
    else if(state == memory_error){
        printf("Memory error.\n");
    }
    else if(state == after_function){
        printf("\nChoose option:\n 1. Back to menu\n 2. Exit\nEnter a number: ");
    }
    else if(state == same){
        printf("\nNew information is like old.\n");
    }
}