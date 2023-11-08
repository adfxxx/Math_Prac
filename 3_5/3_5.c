#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum errors{
    success = 1,
    memory_error = -1,
    fail = -2,
    wrong_option = -3,
    wrong_input = -4,
    is_not_open = -5
};

enum states{
    menu = 1,
    after_function = 2,
    find = 3,
    sort = 4
};

typedef struct{
    int id;
    char *name;
    char *surname;
    char *group;
    unsigned char *grades;
}Student;

int reading(FILE *input, Student **students, int *number_students);
int check_name(const char *name);
int find_student_id(Student *students, int id, Student ***found_students, int flag, int number_students, FILE *output);
int find_student_str(Student *students, const char *str, Student ***found_students, int flag, int number_students);
int compare_students_id(const void *first, const void *second);
int compare_students_surname(const void *first, const void *second);
int compare_students_name(const void *first, const void *second);
int compare_students_group(const void *first, const void *second);
int find_best_student(Student *students, Student ***found_students, int number_students, FILE *output);
double average_grade(Student *students, int number_students);
void delete_students(Student *students, int number_students);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 3){
        print(wrong_input);
        return wrong_input;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open);
        return is_not_open;
    }
    FILE *output = fopen(argv[2], "w");
    if(!output){
        print(is_not_open);
        fclose(input);
        return is_not_open;
    }

    Student *students = NULL;
    int number_students = 0;
    int result = reading(input, &students, &number_students);
    if(result == memory_error){
        print(memory_error);
        delete_students(students, number_students);
        fclose(input);
        fclose(output);
        return memory_error;
    }

    int option = 0;
    int flag = 0;
    char str[256];
    int id = 0;
    Student **found_students = NULL;
    int count = 0;
    while (option != 4){
        fflush(stdin);
        print(menu);
        if(!scanf("%d", &option) || option < 1 || option > 4){
            print(wrong_option);
            print(menu);
            while(!scanf("%d", &option) || option < 1 || option > 4){
                fflush(stdin);
                print(wrong_option);
                print(menu);
            }
        }
        switch (option){
        case 1:
            if(number_students == 0){
                printf("There are no students in the file\n");
            }
            else{
                fflush(stdin);
                print(find);
                if(!scanf("%d", &flag) || flag < 1 || flag > 4){
                    print(wrong_option);
                    break;
                } 
                if (flag == 1){
                    printf("Input id: ");
                    if(!scanf("%d", &id) || id < 0){
                        print(wrong_input);
                        break;
                    }
                    count = find_student_id(students, id, &found_students, flag, number_students, output);
                }
                else{
                    if(flag == 2){
                        printf("Input surname: ");
                    }
                    else if(flag == 3){
                        printf("Input name: ");
                    }
                    else{
                        printf("Input group: ");
                    }
                    if(!scanf("%s", str)){
                        print(wrong_input);
                        break;
                    }
                    count = find_student_str(students, str, &found_students, flag, number_students);
                }
                if(count == memory_error){
                    print(memory_error);
                    delete_students(students, number_students);
                    fclose(input);
                    fclose(output);
                    return memory_error;
                }
                for(int i = 0; i < count; i++){
                    printf("Information about student: \n");
                    printf("\tId: %d, name: %s, surname: %s, group: %s, grades: %c %c %c %c %c\n", 
                        found_students[i]->id, found_students[i]->name, found_students[i]->surname, found_students[i]->group,
                        found_students[i]->grades[0], found_students[i]->grades[1], found_students[i]->grades[2],
                        found_students[i]->grades[3], found_students[i]->grades[4]);
                }
                free(found_students);
                if (count == 0){
                    printf("There is no student with this data.\n");
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
                option = 4;
            }
            break;
        case 2:
            if(number_students == 0){
                printf("There are no students in the file\n");
            }
            else{
                fflush(stdin);
                print(sort);
                if(!scanf("%d", &flag) || flag < 1 || flag > 4){
                    print(wrong_option);
                    break;
                }
                if(flag == 1){
                    qsort(students, number_students, sizeof(Student), compare_students_id);
                }
                else if(flag == 2){
                    qsort(students, number_students, sizeof(Student), compare_students_surname);
                }
                else if(flag == 3){
                    qsort(students, number_students, sizeof(Student), compare_students_name);
                }
                else{
                    qsort(students, number_students, sizeof(Student), compare_students_group);
                }
                for(int i = 0; i < number_students; i++){
                    printf("Information about student: \n");
                    printf("\tId: %d, name: %s, surname: %s, group: %s, grades: %c %c %c %c %c\n", 
                        students[i].id, students[i].name, students[i].surname, students[i].group,
                        students[i].grades[0], students[i].grades[1], students[i].grades[2],
                        students[i].grades[3], students[i].grades[4]);
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
                option = 4;
            }
            break;
        case 3:
            if(number_students == 0){
                printf("There are no students in the file\n");
            }
            else{
                count = find_best_student(students, &found_students, number_students, output);
                if(count == memory_error){
                    print(memory_error);
                    delete_students(students, number_students);  
                    free(found_students);
                    fclose(input);
                    fclose(output);
                    return memory_error;
                }
                printf("Information about the best students:\n");
                for(int i = 0; i < count; i++){
                    printf("\tName: %s, surname: %s\n", found_students[i]->name, found_students[i]->surname);
                }
                free(found_students);
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
                option = 4;
            }
            break;
        }
    }
    delete_students(students, number_students);
    fclose(input);
    fclose(output);
    printf("Goodbye!");
    return success; 

}

int find_best_student(Student *students, Student ***found_students, int number_students, FILE *output){
    double average = average_grade(students, number_students);
    *found_students = (Student **)malloc(sizeof(Student *));
    if(*found_students == NULL){
        return memory_error;
    }
    int count = 0;
    for (int i = 0; i < number_students; i++){
        double sum = 0.0;
        for(int j = 0; j < 5; j++){
            sum += (students[i].grades[j] - '0');
        }
        if(sum/5.0 > average){
            count++;
            Student **temp = (Student **)realloc(*found_students, sizeof(Student **)*count);
            if(temp == NULL){
                return memory_error;
            }
            *found_students = temp;
            (*found_students)[count - 1] = &students[i];
            fprintf(output, "One of the best students:\n \tName: %s, surname: %s\n", students[i].name, students[i].surname);
        }
    }
    return count;
}

double average_grade(Student *students, int number_students){
    double sum = 0.0;
    for(int i = 0; i < number_students; i++){
        for(int j = 0; j < 5; j++){
            sum += (students[i].grades[j] - '0');
        }
    }
    return sum/(number_students*5);
}

double average_grade_student(Student *students, int index){
    double sum = 0.0;
    for(int i = 0; i < 5; i++){
        sum += (students[index].grades[i] - '0');
    }
    return sum/5.0;
}

int compare_students_id(const void *first, const void *second){
    Student *student_first = (Student *)first;
    Student *student_second = (Student *)second;
    if(student_first->id > student_second->id){
        return 1;
    }
    else if(student_first->id < student_second->id){
        return -1;
    }
    else{
        return 0;
    }
}

int compare_students_group(const void *first, const void *second){
    Student *student_first = (Student *)first;
    Student *student_second = (Student *)second;
    return strcmp(student_first->group, student_second->group);
}

int compare_students_name(const void *first, const void *second){
    Student *student_first = (Student *)first;
    Student *student_second = (Student *)second;
    return strcmp(student_first->name, student_second->name);
}

int compare_students_surname(const void *first, const void *second){
    Student *student_first = (Student *)first;
    Student *student_second = (Student *)second;
    return strcmp(student_first->surname, student_second->surname);
}

int find_student_id(Student *students, int id, Student ***found_students, int flag, int number_students, FILE *output){
    Student **temp = NULL;
    int count = 0;
    for(int i = 0; i < number_students; i++){
        if(students[i].id == id){
            count++;
            Student **new_temp = (Student**)realloc(temp, count*sizeof(Student*));
            if(new_temp == NULL){
                for(int j = 0; j < count - 1; j++){
                    free(temp[j]);
                }
                free(temp);
                return memory_error;
            }
            temp = new_temp;
            temp[count-1] = &(students[i]);
            fprintf(output, "Student found by id:\n \tName: %s, surname: %s, group: %s, average grade: %lf\n", 
            students[i].name, students[i].surname, students[i].group, average_grade_student(students, i));
        }
    }
    *found_students = temp;
    return count;
}

int find_student_str(Student *students, const char *str, Student ***found_students, int flag, int number_students){
    Student **temp = NULL;
    int count = 0;
    switch(flag){
        case 2:
        for(int i = 0; i < number_students; i++){
            if(!strcmp(students[i].surname, str)){
                count++;
                Student **new_temp = (Student**)realloc(temp, count*sizeof(Student*));
                if(new_temp == NULL){
                    for(int j = 0; j < count - 1; j++){
                        free(temp[j]);
                    }
                    free(temp);
                    return memory_error;
                }
                temp = new_temp;
                temp[count-1] = &(students[i]);
            }
        }
        break;
        case 3:
        for(int i = 0; i < number_students; i++){
            if(!strcmp(students[i].name, str)){
                count++;
                Student **new_temp = (Student**)realloc(temp, count*sizeof(Student*));
                if(new_temp == NULL){
                    for(int j = 0; j < count - 1; j++){
                        free(temp[j]);
                    }
                    free(temp);
                    return memory_error;
                }
                temp = new_temp;
                temp[count-1] = &(students[i]);
            }
        }
        break;
        case 4:
        for(int i = 0; i < number_students; i++){
            if(!strcmp(students[i].group, str)){
                count++;
                Student **new_temp = (Student**)realloc(temp, count*sizeof(Student*));
                if(new_temp == NULL){
                    for(int j = 0; j < count - 1; j++){
                        free(temp[j]);
                    }
                    free(temp);
                    return memory_error;
                }
                temp = new_temp;
                temp[count-1] = &(students[i]);
            }
        }
        break;
    }
    *found_students = temp;
    return count;
}

int reading(FILE *input, Student **students, int *number_students){
    int count = 10;
    *students = (Student *)malloc(count*sizeof(Student));
    if(*students == NULL){
        return memory_error;
    }
    *number_students = 0;
    char *line = NULL;
    size_t line_length = 0;
    while(getline(&line, &line_length, input) != -1){
        if(*number_students == count){
            count *= 2;
            Student *temp = (Student*)realloc(*students, count*sizeof(Student));
            if(temp == NULL){
                free(line);
                return (memory_error);
            }
            *students = temp;
        }
        int id;
        char name[256];
        char surname[256];
        char group[256];
        char grades[5];
        int flag = 1;
        if(sscanf(line, "%d %255s %255s %255s %c %c %c %c %c", 
        &id, name, surname, group, &grades[0], &grades[1], &grades[2], &grades[3], &grades[4]) == 9){
            Student *cur_student = &(*students)[*number_students];
            cur_student->grades = (unsigned char*)malloc(5*sizeof(unsigned char));
            if (cur_student->grades == NULL){
                return memory_error;
            }
            for(int i = 0; i < 5; i++){
                if((grades[i] - '0') > 5 || (grades[i] - '0') < 2){
                    flag = 0;
                    break;
                }
                else{
                    cur_student->grades[i] = (unsigned char)grades[i];
                }
            }
            if(flag == 0){
                free(cur_student->grades);
                continue;
            }
            if(check_name(name) == fail || check_name(surname) == fail){
                free(cur_student->grades);
                continue;
            }
            if(strlen(group) == 0){
                free(cur_student->grades);
                continue;
            }
            if(id < 0){
                free(cur_student->grades);
                continue;
            }
            cur_student->id = id;
            cur_student->name = (char*)malloc((strlen(name) + 1)*sizeof(char));
            if(cur_student->name == NULL){
                free(line);
                return memory_error;
            }
            cur_student->surname = (char*)malloc((strlen(surname) + 1)*sizeof(char));
            if(cur_student->surname == NULL){
                free(line);
                return memory_error;
            }
            cur_student->group = (char*)malloc((strlen(group) + 1)*sizeof(char));
            if(cur_student->group == NULL){
                free(line);
                return memory_error;
            }
            strcpy(cur_student->name, name);
            strcpy(cur_student->surname, surname);
            strcpy(cur_student->group, group);
            (*number_students)++;
        }
    }
    free(line);
    return success;
}

void delete_students(Student *students, int number_students){
    for (int i = 0; i < number_students; i++) {
        free(students[i].name);
        free(students[i].surname);
        free(students[i].group);
        free(students[i].grades);
    }
    free(students);
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

void print(int state){
    if(state == menu){
        printf("\tMAIN MENU\nChoose option:\n 1. Find student\n 2. Sort students\n 3. Print best students\n 4. Exit\nEnter a number: ");
    }
    else if(state == wrong_option){
        printf("Wrong option. Try again.\n");
    }
    else if(state == wrong_input){
        printf("Wrong input. Try again.\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == after_function){
        printf("Choose option:\n 1. Back to menu\n 2. Exit\nEnter a number: ");
    }
    else if(state == is_not_open){
        printf("File is not open.\n");
    }
    else if(state == find){
        printf("Choose an option:\n 1. Find by id\n 2. Find by surname\n 3. Find by name\n 4. Find by group\nEnter a number: ");
    }
    else if(state == sort){
        printf("Choose an option:\n 1. Sort by id\n 2. Sort by surname\n 3. Sort by name\n 4. Sort by group\nEnter a number: ");
    }
}