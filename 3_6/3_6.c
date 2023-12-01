#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4
};

enum options{
    menu = 2,
    after_function = 3
};

typedef struct{
    char *time_stop;
    char *time_start;
    char *number;
    char mark;
    double coords[2];
}Info;

typedef struct Stop_node{
    Info stop;
    struct Stop_node *next;
}Stop_node;

typedef struct List{
    Stop_node *head;
}List;

int find_longest_way(List *routes, Stop_node **max_stop, Stop_node **min_stop);
int find_longest_route(List *routes, Stop_node **max_stop, Stop_node **min_stop);
int find_routes_number(List *routes, Stop_node **max_stop, Stop_node **min_stop);
int find_longest_stop(List *routes, Stop_node **max_stop);
int get_duration(const char *start, const char *stop);
int get_time(List *roots, Stop_node **max_stop, Stop_node **min_stop, double x, double y);
int change_command(List *routes, double *coord, int count);
void print_routes(List *routes);
Stop_node *create_stop(Info stop);
void free_list(Stop_node *head);
time_t convert_time(const char *time);
void insert(Stop_node **head, Info stop);
int read_file(FILE *input, List *routes, double **coord, int *size, int *count);
int check_dif(int day_stop, int month_stop, int year_stop, int hour_stop, int min_stop, int sec_stop,
              int day_start, int month_start, int year_start, int hour_start, int min_start, int sec_start);
int check_time(int day, int month, int year, int hour, int min, int sec);
void print(int state);

int main(int argc, char *argv[]){
    if(argc < 2){
        print(wrong_input);
        return wrong_input;
    }

    List routes;
    routes.head = NULL;
    int size = 100;
    double *coord = (double*)malloc(size*sizeof(double));
    if(coord == NULL){
        return memory_error;
    }
    int result;
    int count = 0;
    for(int i = 1; i < argc; i++){
        FILE *file = fopen(argv[i], "r");
        if(!file){
            print(is_not_open_input);
            return is_not_open_input;
        }
        result = read_file(file, &routes, &coord, &size, &count);
        if(result == memory_error){
            free(coord);
            fclose(file);
            print(memory_error);
            if(routes.head != NULL){
                free_list(routes.head);
            }
            return memory_error;
        }
        fclose(file);
    }
    if(change_command(&routes, coord, count/2) == memory_error){
        free(coord);
        if(routes.head != NULL){
            free_list(routes.head);
        }
        print(memory_error);
        return memory_error;
    }
    printf("Goodbye!");
    free(coord);
    if(routes.head != NULL){
        free_list(routes.head);
    }
    return success;
}

int find_longest_way(List *routes, Stop_node **max_stop, Stop_node **min_stop){
    Stop_node *cur = routes->head;
    *max_stop = create_stop(cur->stop);
    if(*max_stop == NULL){
        return memory_error;
    }
    *min_stop = create_stop(cur->stop);
    if(*min_stop == NULL){
        return memory_error;
    }
    int M;
    int S;
    int F;
    int max_routes = 0;
    int min_routes = 100;
    int total;
    char *transport;
    while(cur != NULL){
        S = 0;
        M = 0;
        F = 0;
        total = 0;
        transport = cur->stop.number;
        Stop_node *temp_cur = cur;
        while(temp_cur != NULL){
            if(!strcmp(temp_cur->stop.number, transport)){
                if(temp_cur->stop.mark == 'S'){
                    S++;
                }
                else if(temp_cur->stop.mark == 'M'){
                    M++;
                }
                else{
                    F++;
                }
            }
            temp_cur = temp_cur->next;
        }
        total = S + M + F;
        if(total >= max_routes){
            max_routes = total;
            free_list(*max_stop);
            *max_stop = create_stop(cur->stop);
            if(*max_stop == NULL){
                return memory_error;
            }
        }
        if(total <= min_routes){
            min_routes = total;
            free_list(*min_stop);
            *min_stop = create_stop(cur->stop);
            if(*min_stop == NULL){
                return memory_error;
            }
        }
        cur = cur->next;
    }
}

int find_longest_route(List *routes, Stop_node **max_stop, Stop_node **min_stop){
    Stop_node *cur = routes->head;
    *max_stop = create_stop(cur->stop);
    if(*max_stop == NULL){
        return memory_error;
    }
    *min_stop = create_stop(cur->stop);
    if(*min_stop == NULL){
        return memory_error;
    }
    int M;
    int S;
    int F;
    int max_routes = 0;
    int min_routes = 100;
    char *transport;
    while(cur != NULL){
        S = 0;
        M = 0;
        F = 0;
        transport = cur->stop.number;
        Stop_node *temp_cur = cur;
        while(temp_cur != NULL){
            if(!strcmp(temp_cur->stop.number, transport)){
                if(temp_cur->stop.mark == 'S'){
                    S++;
                }
                else if(temp_cur->stop.mark == 'M'){
                    M++;
                }
                else{
                    F++;
                }
            }
            temp_cur = temp_cur->next;
        }
        if(S == F){
            if(M >= max_routes){
                max_routes = M;
                free_list(*max_stop);
                *max_stop = create_stop(cur->stop);
                if(*max_stop == NULL){
                    return memory_error;
                }
            }
            if(M <= min_routes){
                min_routes = M;
                free_list(*min_stop);
                *min_stop = create_stop(cur->stop);
                if(*min_stop == NULL){
                    return memory_error;
                }
            }
        }
        cur = cur->next;
    }
}

int find_routes_number(List *routes, Stop_node **max_stop, Stop_node **min_stop){
    Stop_node *cur = routes->head;
    *max_stop = create_stop(cur->stop);
    if(*max_stop == NULL){
        return memory_error;
    }
    *min_stop = create_stop(cur->stop);
    if(*min_stop == NULL){
        free_list(*max_stop);
        return memory_error;
    }
    int max_routes = 0;
    int min_routes = 100;
    char *transport;
    int S;
    int M;
    int F;
    int total;
    while (cur != NULL){
        transport = cur->stop.number;
        S = 0;
        F = 0;
        Stop_node *temp_cur = cur;
        while(temp_cur != NULL){
            if(!strcmp(temp_cur->stop.number, transport)){
                if(temp_cur->stop.mark == 'S'){
                    S++;
                }
                else if(temp_cur->stop.mark == 'F'){
                    F++;
                }
            }
            temp_cur = temp_cur->next;
        }
        if(S == F){
            if(S >= max_routes){
                max_routes = S;
                free_list(*max_stop);
                *max_stop = create_stop(cur->stop);
                if(*max_stop == NULL){
                    return memory_error;
                }
            }
            if(S <= min_routes){
                min_routes = S;
                free_list(*min_stop);
                *min_stop = create_stop(cur->stop);
                if(*min_stop == NULL){
                    return memory_error;
                }
            }
        }
        cur = cur->next;
    }
}

int find_longest_stop(List *routes, Stop_node **max_stop){
    Stop_node *cur = routes->head;
    *max_stop = create_stop(cur->stop);
    if(*max_stop == NULL){
        return memory_error;
    }
    int total;
    int duration;
    int max_sp =  get_duration(cur->stop.time_start, cur->stop.time_stop);
    while(cur != NULL){
        total = 0;
        Stop_node *cur_veh = routes->head;
        while(cur_veh != NULL){
            if(!strcmp(cur->stop.number, cur_veh->stop.number)){
                duration = get_duration(cur_veh->stop.time_start, cur_veh->stop.time_stop);
                total += duration;
            }
            cur_veh = cur_veh->next;
        }
        if(total < max_sp){
            max_sp = total;
            free((*max_stop)->stop.number);
            (*max_stop)->stop.number = strdup(cur->stop.number);
        }
        cur = cur->next;
    }
}

int get_duration(const char *start, const char *stop){
    time_t start_time = convert_time(start);
    time_t stop_time = convert_time(stop);
    if(start_time != -1 && stop_time != -1){
        return difftime(stop_time, start_time);
    }
    return -1;
}

int get_time(List *roots, Stop_node **max_stop, Stop_node **min_stop, double x, double y){
    Stop_node *cur = roots->head;
    int result;
    int max_sp = 0;
    int min_sp = 0;
    while(cur != NULL){
        if(cur->stop.coords[0] == x && cur->stop.coords[1] == y){
            if(max_sp == 0){
                *max_stop = create_stop(cur->stop);
                if(*max_stop == NULL){
                    return memory_error;
                }
                *min_stop = create_stop(cur->stop);
                if(*min_stop == NULL){
                    free_list(*max_stop);
                    return memory_error;
                }
                max_sp = get_duration(cur->stop.time_start, cur->stop.time_stop);
                min_sp = max_sp;
            }
            result = get_duration(cur->stop.time_start, cur->stop.time_stop);
            if(result < max_sp){
                max_sp = result;
                free((*max_stop)->stop.number);
                (*max_stop)->stop.number = strdup(cur->stop.number);
            }
            else if(result > min_sp){
                min_sp = result;
                free((*min_stop)->stop.number);
                (*min_stop)->stop.number = strdup(cur->stop.number);
            }
        }
        cur = cur->next;
    }
}

int change_command(List *routes, double *coord, int count){
    int option = 0;
    int number;
    double x;
    double y;
    int i = 0;
    Stop_node *max_stop;
    Stop_node *min_stop;
    while(option != 7){
        option = 0;
        print(menu);
        if(!scanf("%d", &option) || option < 1 || option > 7){
            print(wrong_input);
            print(menu);
            fflush(stdin);
            while(!scanf("%d", &option) || option < 1 || option > 7){
                fflush(stdin);
                print(wrong_input);
                print(menu);
            }
        }
        switch(option){
            case 1:
            if(routes->head == NULL){
                printf("The list is empty\n");
            }
            else{
                if(find_routes_number(routes, &max_stop, &min_stop) == memory_error){
                    free_list(max_stop);
                    free_list(min_stop);
                    return memory_error;
                }
                printf("\nTransport with the biggest number of routes: %s\n", max_stop->stop.number);
                printf("Transport with the least number of routes: %s\n", min_stop->stop.number);
                free_list(max_stop);
                free_list(min_stop);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 2:
            if(routes->head == NULL){
                printf("The list is empty\n");
            }
            else{
                if(find_routes_number(routes, &max_stop, &min_stop) == memory_error){
                    free_list(max_stop);
                    free_list(min_stop);
                    return memory_error;
                }
                printf("\nTransport with the longest way: %s\n", max_stop->stop.number);
                printf("Transport with the shortest way: %s\n", min_stop->stop.number);
                free_list(max_stop);
                free_list(min_stop);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 3:
            if(routes->head == NULL){
                printf("The list is empty\n");
            }
            else{
                if(find_longest_route(routes, &max_stop, &min_stop) == memory_error){
                    free_list(max_stop);
                    free_list(min_stop);
                    return memory_error;
                }
                printf("\nTransport with the longest route: %s\n", max_stop->stop.number);
                printf("Transport with the shortest route: %s\n", min_stop->stop.number);
                free_list(max_stop);
                free_list(min_stop);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 4:
            if(routes->head == NULL){
                printf("The list is empty\n");
            }
            else{
                while(count != 0){
                    x = coord[i];
                    y = coord[i+1];
                    count--;
                    i += 2;
                    if(get_time(routes, &max_stop, &min_stop, x, y) == memory_error){
                        free_list(max_stop);
                        free_list(min_stop);
                        return memory_error;
                    }
                    printf("Stop %lf %lf:", x, y);
                    printf("\n\tTransport with the longest stop: %s\n", max_stop->stop.number);
                    printf("\tTransport with the shortest stop: %s\n", min_stop->stop.number);
                    free_list(max_stop);
                    free_list(min_stop);
                }
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 5:
            if(routes->head == NULL){
                printf("The list is empty\n");
            }
            else{
                if(find_longest_stop(routes, &max_stop) == memory_error){
                    free_list(max_stop);
                    return memory_error;
                }
                printf("\nTransport with the longest stop: %s\n", max_stop->stop.number);
                free_list(max_stop);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
            case 6:
            if(routes->head == NULL){
                printf("The list is empty\n");
            }
            else{
                print_routes(routes);
            }
            print(after_function);
            fflush(stdin);
            if(!scanf("%d", &option) || option < 1 || option > 2){
                print(wrong_input);
                print(after_function);
                fflush(stdin);
                while(!scanf("%d", &option) || option < 1 || option > 2){
                    fflush(stdin);
                    print(wrong_input);
                    print(after_function);
                }
            }
            if (option == 2){
                option = 7;
            }
            break;
        }
    }
    return success;
}

void print_routes(List *routes){
    Stop_node *cur = routes->head;
    while (cur != NULL) {
        printf("Number: %s, start time: %s, stop time: %s, stop: %lf %lf, mark: %c\n", cur->stop.number, cur->stop.time_start, cur->stop.time_stop, cur->stop.coords[0], cur->stop.coords[1], cur->stop.mark);
        printf("---------------\n");
        cur = cur->next;
    }
}

Stop_node *create_stop(Info stop){
    Stop_node *new_node = (Stop_node*)malloc(sizeof(Stop_node));
    if(new_node == NULL){
        return NULL;
    }
    new_node->stop = stop;
    new_node->stop.number = strdup(stop.number);
    new_node->stop.time_start = strdup(stop.time_start);
    new_node->stop.time_stop = strdup(stop.time_stop);
    new_node->next = NULL;
    return new_node;
}

time_t convert_time(const char *time){
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    sscanf(time, "%d.%d.%d %d:%d:%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900;
    tm.tm_mon--;
    return mktime(&tm);
}

void insert(Stop_node **head, Info stop){
    Stop_node *new_node = create_stop(stop);
    if(*head == NULL || convert_time(stop.time_stop) < convert_time((*head)->stop.time_stop)){
        new_node->next = *head;
        *head = new_node;
    }
    else{
        Stop_node *cur = *head;
        while(cur->next != NULL && convert_time(stop.time_stop) > convert_time(cur->stop.time_stop)){
            cur = cur->next;
        }
        new_node->next = cur->next;
        cur->next = new_node;
    }
}

int read_file(FILE *input, List *routes, double **coord, int *size, int *count){
    char *line = NULL;
    size_t len = 0;
    double coord_x;
    double coord_y;
    if(getline(&line, &len, input) != -1){
        if(sscanf(line, "%lf %lf", &coord_x, &coord_y) != 2){
            free(line);
            return wrong_input;
        }
    }
    else{
        free(line);
        return wrong_input;
    }
    (*coord)[*count] = coord_x;
    (*count)++;
    (*coord)[*count] = coord_y;
    (*count)++;
    char *number = (char*)malloc(20*sizeof(char));
    if(number == NULL){
        free(line);
        return memory_error;
    }
    int day_stop;
    int month_stop;
    int year_stop;
    int hour_stop;
    int min_stop;
    int sec_stop;
    int day_start;
    int month_start;
    int year_start;
    int hour_start;
    int min_start;
    int sec_start;
    char mark;
    char *time_start = (char*)malloc(20*sizeof(char));
    if(time_start == NULL){
        free(line);
        free(number);
        return memory_error;
    }
    time_start[20] = '\0';
    char *time_stop = (char*)malloc(20*sizeof(char));
    if(time_stop == NULL){
        free(line);
        free(time_start);
        free(number);
        return memory_error;
    }
    time_stop[20] = '\0';
    if(*count == 100){
        (*size)*=2;
        double *temp = (double*)realloc(*coord, (*size)*sizeof(double));
        if(temp == NULL){
            free(number);
            free(time_start);
            free(time_stop);
            free(line);
            return memory_error;
        }
        *coord = temp;
    }
    Info stop;
    while(getline(&line, &len, input) != -1){
        if(sscanf(line, "%s %d.%d.%d %d:%d:%d %d.%d.%d %d:%d:%d %c", number, &day_stop, &month_stop, &year_stop, &hour_stop, &min_stop, &sec_stop,
        &day_start, &month_start, &year_start, &hour_start, &min_start, &sec_start, &mark) != 14){
            continue;
        }
        else{
            if(check_time(day_stop, month_stop, year_stop, hour_stop, min_stop, sec_stop) == fail){
                continue;
            }
            if(check_time(day_start, month_start, year_start, hour_start, min_start, sec_start) == fail){
                continue;
            }
            if(check_dif(day_stop, month_stop, year_stop, hour_stop, min_stop, sec_stop,
            day_start, month_start, year_start, hour_start, min_start, sec_start) == fail){
                continue;
            }
            if(mark != 'S' && mark != 'F' && mark != 'M'){
                continue;
            }
            sprintf(time_stop, "%d.%d.%d %d:%d:%d", day_stop, month_stop, year_stop, hour_stop, min_stop, sec_stop);
            sprintf(time_start, "%d.%d.%d %d:%d:%d", day_start, month_start, year_start, hour_start, min_start, sec_start);
            stop.coords[0] = coord_x;
            stop.coords[1] = coord_y;
            stop.time_start = strdup(time_start);
            stop.time_stop = strdup(time_stop);
            stop.mark = mark;
            stop.number = strdup(number); 
            insert(&routes->head, stop);
        }
    }
    free(number);
    free(time_start);
    free(time_stop);
    free(line);
    
    return success;
}

int check_dif(int day_stop, int month_stop, int year_stop, int hour_stop, int min_stop, int sec_stop, int day_start, int month_start, int year_start, int hour_start, int min_start, int sec_start){
    if(year_start < year_stop){
        return fail;
    }
    else if(year_start == year_stop){
        if(month_start < month_stop){
            return fail;
        }
        else if(month_start == month_stop){
            if(day_start < day_stop){
                return fail;
            }
            else if(day_start == day_stop){
                if(hour_start < hour_stop){
                    return fail;
                }
                else if(hour_start == hour_stop){
                    if(min_start < min_stop){
                        return fail;
                    }
                    else if(min_start == min_stop){
                        if(sec_start < sec_stop){
                            return fail;
                        }
                        else if(sec_start == sec_stop){
                            return fail;
                        }
                    }
                }
            }
        }
    }
    return success;
}

int check_time(int day, int month, int year, int hour, int min, int sec){
    if(day > 31 || day < 1){
        return fail;
    }
    if(month > 12 || month < 1){
        return fail;
    }
    if(month == 2 && day > 29){
        return fail;
    }
    if(year < 0){
        return fail;
    }
    if(hour > 23 || hour < 0){
        return fail;
    }
    if(min > 59 || min < 0){
        return fail;
    }
    if(sec > 59 || sec < 0){
        return fail;
    }
    return success;
}

void free_list(Stop_node *head){
    Stop_node *cur = head;
    Stop_node *next;
    while(cur != NULL){
        next = cur->next;
        free(cur->stop.time_start);
        free(cur->stop.time_stop);
        free(cur->stop.number);
        free(cur);

        cur = next;
    }
}

void print(int state){
    if(state == wrong_input){
        printf("\nWrong input. Try again.\n");
    }
    else if(state == memory_error){
        printf("Memory error\n");
    }
    else if(state == is_not_open_input){
        printf("File with data is not open\n");
    }
    else if(state == menu){
        printf("\n\tMAIN MENU\n 1.Find the most/least number of routes\n 2.Find the longest/shortest way\n 3.Find the longest/shortest rout\n 4.Find the longest/shortes stop\n 5.The longest stand\n 6.Print list\n 7.Exit\nEnter a number: ");
    }
    else if(state == after_function){
        printf("\nChoose option:\n 1. Back to menu\n 2. Exit\nEnter a number: ");
    }
}