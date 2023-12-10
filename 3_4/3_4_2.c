#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

enum errors{
    success = 1,
    memory_error = -1,
    fail = -2,
    wrong_option = -3,
    wrong_input = -4,
    id_error = -5,
    fail_time = -6
};

enum states{
    menu = 1,
    after_function = 2
};

typedef struct{
    char *string;
    int length;
} String;

typedef struct{
    String city;
    String street;
    int house;
    String building;
    int apartment;
    String index;
}Address;

typedef struct{
    Address address_client;
    double weight;
    String id;
    String creation_time;
    String delivery_time;
}Mail;

typedef struct{
    Address post_address;
    Mail *packages;
    int packages_number;
}Post;

int compare_package_time(const void *first, const void *second);
Mail** get_delivered_packages(Post *post, int *count);
void get_time(const char *delivery_time, struct tm *delivery_tm);
int delete_package(Post *post, const char *input_id);
int find_mail(Post *post, const char *input_id, Mail ***found_mails);
int add_package(Post *post, Address address_client, double weight, String id, String creation_time, String delivery_time);
int get_string(String *str, int flag);
int is_valid_index(const String *str);
int is_valid_id(const String *str);
int is_valid_time(const String *str);
void delete(String *str);
void delete_packages(Post *post);
void print(int state);

int main(){
    int option = 0;
    int result = 0;
    double eps = 0.000000001;
    Address address_client;
    double weight;
    String id;
    String creation_time;
    String delivery_time;
    Post post;
    String input_id;
    int count = 0;
    Mail **found_mail = NULL;
    Mail **found_mail_delivered = NULL;
    post.packages_number = 0;
    post.packages = NULL;
    while (option != 6){
        fflush(stdin);
        print(menu);
        if(!scanf("%d", &option) || option < 1 || option > 6){
            print(wrong_option);
            print(menu);
            while(!scanf("%d", &option) || option < 1 || option > 6){
                fflush(stdin);
                print(wrong_option);
                print(menu);
            }
        }
        switch (option){
        case 1:
            fflush(stdin);
            printf("Enter city: ");
            result = get_string(&address_client.city, 0);
            if(result == wrong_input){
                print(wrong_input);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                option = 6;
                break;
            }
            fflush(stdin);
            printf("Enter street: ");
            result = get_string(&address_client.street, 0);
            if(result == wrong_input){
                print(wrong_input);
                free(address_client.city.string);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                free(address_client.city.string);
                option = 6;
                break;
            }
            fflush(stdin);
            printf("Enter a number of house: ");
            if(!scanf("%d", &address_client.house) || address_client.house <= 0){
                print(wrong_input);
                option = 0;
                break;
            }
            fflush(stdin);
            printf("Enter a building: ");
            result = get_string(&address_client.building, 0);
            if(result == wrong_input){
                print(wrong_input);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                free(address_client.city.string);
                free(address_client.street.string);
                option = 6;
                break;
            }
            fflush(stdin);
            printf("Enter a number of apartment: ");
            if(!scanf("%d", &address_client.apartment) || address_client.apartment <= 0){
                print(wrong_input);
                option = 0;
                break;
            }
            fflush(stdin);
            printf("Enter index: ");
            result = get_string(&address_client.index, 2);
            if(result == wrong_input){
                print(wrong_input);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                free(address_client.city.string);
                free(address_client.street.string);
                free(address_client.building.string);
                option = 6;
                break;
            }
            fflush(stdin);
            printf("Enter weight of package: ");
            if(!scanf("%lf", &weight) || weight <= eps){
                print(wrong_input);
                option = 0;
                break;
            }
            fflush(stdin);
            printf("Enter an id: ");
            result = get_string(&id, 3);
            if(result == wrong_input){
                print(wrong_input);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                free(address_client.city.string);
                free(address_client.street.string);
                free(address_client.building.string);
                free(address_client.index.string);
                option = 6;
                break;
            }
            fflush(stdin);
            printf("Enter creation time (dd:MM:yyyy hh:mm:ss): ");
            result = get_string(&creation_time, 4);
            if(result == wrong_input){
                print(wrong_input);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                free(address_client.city.string);
                free(address_client.street.string);
                free(address_client.building.string);
                free(address_client.index.string);
                free(id.string);
                option = 6;
                break;
            }
            fflush(stdin);
            printf("Enter delivery time (dd:MM:yyyy hh:mm:ss): ");
            result = get_string(&delivery_time, 4);
            if(result == wrong_input){
                print(wrong_input);
                option = 0;
                break;
            }
            else if(result == memory_error){
                print (memory_error);
                free(address_client.city.string);
                free(address_client.street.string);
                free(address_client.building.string);
                free(address_client.index.string);
                free(id.string);
                free(creation_time.string);
                option = 6;
                break;
            }
            fflush(stdin);
            result = add_package(&post, address_client, weight, id, creation_time, delivery_time);
            if(result == memory_error){
                print(memory_error);
                delete_packages(&post);
                return memory_error;
            }
            else if(result == id_error){
                print(id_error);
                option = 0;
                break;
            }
            else if(result == fail_time){
                print(fail_time);
                option = 0;
                break;
            }
            else{
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
                    option = 6;
                }
            }
            break;
        case 2:
            if(post.packages_number == 0){
                printf("There are no packages in the system\n");
            }
            else{
                fflush(stdin);
                printf("Input package's id: ");
                get_string(&input_id, 3);
                count = delete_package(&post, input_id.string);
                if(count == memory_error){
                    print(memory_error);
                    return memory_error;
                }
                else if(count == fail){
                    printf("There is no package with this id in the system\n");
                }
                else{
                    printf("Success.\n");
                }
                delete(&input_id);
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
                option = 6;
            }
            break;
        case 3:
            if(post.packages_number == 0){
                printf("There are no packages in the system\n");
            }
            else{
                fflush(stdin);
                printf("Input package's id: ");
                get_string(&input_id, 3);
                count = find_mail(&post, input_id.string, &found_mail);
                if(count == memory_error){
                    print(memory_error);
                    return memory_error;
                }
                if(count == 1){
                    printf("Information about package: \n");
                    printf("\tAddress: %s city, %s street, house %d, building %s, apartment %d, index %s\n", 
                        found_mail[count-1]->address_client.city.string, found_mail[count-1]->address_client.street.string,
                        found_mail[count-1]->address_client.house, found_mail[count-1]->address_client.building.string,
                        found_mail[count-1]->address_client.apartment, found_mail[count-1]->address_client.index.string);
                    printf("\tWeight of package: %lf\n", found_mail[count-1]->weight);
                    printf("\tTime of creation: %s\n", found_mail[count-1]->creation_time);
                    printf("\tTime of delivery: %s\n", found_mail[count-1]->delivery_time);
                    free(found_mail);
                    }
                else if (count == 0){
                    printf("There is not any package with this id.\n");
                }
                delete(&input_id);
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
                option = 6;
            }
            break;
        case 4:
            if(post.packages_number == 0){
                printf("There are no packages in the system\n");
            }
            else{
                count = 0;
                found_mail_delivered = get_delivered_packages(&post, &count);
                if(found_mail_delivered == NULL){
                    print(memory_error);
                    delete_packages(&post);
                    return memory_error;
                }
                else{
                    qsort(found_mail_delivered, count, sizeof(Mail*), compare_package_time);
                    for(int i = 0; i < count; i++){
                        printf("Information about package: \n");
                        printf("\tAddress: %s city, %s street, house %d, building %s, apartment %d, index %s\n", 
                            found_mail_delivered[i]->address_client.city.string, found_mail_delivered[i]->address_client.street.string,
                            found_mail_delivered[i]->address_client.house, found_mail_delivered[i]->address_client.building.string,
                            found_mail_delivered[i]->address_client.apartment, found_mail_delivered[i]->address_client.index.string);
                        printf("\tWeight of package: %lf\n", found_mail_delivered[i]->weight);
                        printf("\tTime of creation: %s\n", found_mail_delivered[i]->creation_time);
                        printf("\tTime of delivery: %s\n", found_mail_delivered[i]->delivery_time);
                    }
                    for (int i = 0; i < count; i++){
                        free(found_mail_delivered[i]);
                        found_mail_delivered[i] = NULL;
                    }
                    free(found_mail_delivered);
                }
            }
            print(after_function);
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
                option = 6;
            }  
            break;
        case 5:
            for(int i = 0; i < post.packages_number; i++){
                printf("Information about package: \n");
                printf("\tAddress: %s city, %s street, house %d, building %s, apartment %d, index %s\n", 
                    post.packages[i].address_client.city.string, post.packages[i].address_client.street.string,
                    post.packages[i].address_client.house, post.packages[i].address_client.building.string,
                    post.packages[i].address_client.apartment, post.packages[i].address_client.index.string);
                printf("\tWeight of package: %lf\n", post.packages[i].weight);
                printf("\tId of package: %s\n", post.packages[i].id);
                printf("\tTime of creation: %s\n", post.packages[i].creation_time);
                printf("\tTime of delivery: %s\n", post.packages[i].delivery_time);
            }
            print(after_function);
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
                option = 6;
            }
            break;
        }
    }
    printf("Goodbye!");
    delete_packages(&post);
    return success;
}

int compare_package_time(const void *first, const void *second){
    Mail *mail_first = *(Mail **)first;
    Mail *mail_second = *(Mail **)second;
    
    struct tm tm_first, tm_second;
    get_time(mail_first->creation_time.string, &tm_first);
    get_time(mail_second->creation_time.string, &tm_second);
    
    time_t time_first = mktime(&tm_first);
    time_t time_second = mktime(&tm_second);
    
    if (difftime(time_first, time_second) < 0) {
        return -1;
    } else if (difftime(time_first, time_second) > 0) {
        return 1;
    } else {
        return 0;
    }
}

Mail** get_delivered_packages(Post *post, int *count){
    time_t cur_time;
    time(&cur_time);
    Mail **temp = NULL;

    for (int i = 0; i < post->packages_number; i++) {
        struct tm delivery_tm;
        get_time(post->packages[i].delivery_time.string, &delivery_tm);
        time_t d_t = mktime(&delivery_tm);
        if (d_t <= cur_time) {
            (*count)++;
            Mail **new_temp = (Mail**)realloc(temp, (*count) * sizeof(Mail*));
            if (new_temp == NULL) {
                free(temp);
                return NULL;
            }
            temp = new_temp;
            temp[(*count) - 1] = malloc(sizeof(Mail));
            if (temp[(*count) - 1] == NULL) {
                free(temp);
                return NULL;
            }
            memcpy(temp[(*count) - 1], &post->packages[i], sizeof(Mail));
        }
    }
    return temp;
}

void get_time(const char *delivery_time, struct tm *delivery_tm){
    sscanf(delivery_time, "%d:%d:%d %d:%d:%d", 
               &delivery_tm->tm_mday, &delivery_tm->tm_mon, &delivery_tm->tm_year, 
               &delivery_tm->tm_hour, &delivery_tm->tm_min, &delivery_tm->tm_sec);
    delivery_tm->tm_year -= 1900;
    delivery_tm->tm_mon -= 1;
}

int delete_package(Post *post, const char *input_id) {
    int count = -1;

    for (int i = 0; i < post->packages_number; i++) {
        if (!strcmp(post->packages[i].id.string, input_id)) {
            count = i;
            break;
        }
    }
    if (count == -1) {
        return fail; 
    }

    delete(&post->packages[count].address_client.city);
    delete(&post->packages[count].address_client.street);
    delete(&post->packages[count].address_client.building);
    delete(&post->packages[count].address_client.index);
    delete(&post->packages[count].id);
    delete(&post->packages[count].creation_time);
    delete(&post->packages[count].delivery_time);

    for (int i = count; i < post->packages_number - 1; i++) {
        post->packages[i] = post->packages[i + 1];
    }

    post->packages_number--;

    Mail *temp = (Mail *)realloc(post->packages, post->packages_number * sizeof(Mail));
    
    if (temp == NULL && post->packages_number > 0) {
        return memory_error; 
    }

    post->packages = temp;

    return success;
}


int find_mail(Post *post, const char *input_id, Mail ***found_mails){
    Mail **temp = NULL;
    int count = 0;

    for(int i = 0; i < post->packages_number; i++){
        if(!strcmp(post->packages[i].id.string, input_id)){
            count++;
            Mail **new_temp = (Mail**)realloc(temp, count*sizeof(Mail*));
            if(new_temp == NULL){
                for(int j = 0; j < count - 1; j++){
                    free(temp[j]);
                }
                free(temp);
                return memory_error;
            }
            temp = new_temp;
            temp[count-1] = &(post->packages[i]);
        }
    }
    *found_mails = temp;
    return count;
}

int add_package(Post *post, Address address_client, double weight, String id, String creation_time, String delivery_time){
    struct tm tm_creation, tm_delivery;
    get_time(creation_time.string, &tm_creation);
    get_time(delivery_time.string, &tm_delivery);
    time_t time_creation = mktime(&tm_creation);
    time_t time_delivery = mktime(&tm_delivery);
    if(difftime(time_delivery, time_creation) < 0){
        return fail_time;
    }
    for(int i = 0; i < post->packages_number; i++){
        if(!strcmp(post->packages[i].id.string, id.string)){
            return id_error;
        }
    }

    Mail new_mail;
    new_mail.address_client = address_client;
    new_mail.weight = weight;
    new_mail.id = id;
    new_mail.creation_time = creation_time;
    new_mail.delivery_time = delivery_time;
    
    int index = 0;
    int check_index = 0;
    int check_id = 0;
    while (index < post->packages_number){
        Mail *cur_mail = &post->packages[index];
        check_index = strcmp(new_mail.address_client.index.string, cur_mail->address_client.index.string);
        if(check_index < 0){
            break;
        }
        else if(check_index == 0){
            check_id = strcmp(new_mail.id.string, cur_mail->id.string);
            if(check_id < 0){
                break;
            }
        }
        index++;
    }
    post->packages_number++;
    Mail *temp_packages = (Mail*)realloc(post->packages, post->packages_number*sizeof(Mail));
    if(temp_packages == NULL){
        return memory_error;
    }
    post->packages = temp_packages;
    for(int i = post->packages_number-1; i > index; i--){
        post->packages[i] = post->packages[i-1];
    }
    post->packages[index] = new_mail;

    return success;
}

int get_string(String *str, int flag){
    int length = 2;
    int i = 0;
    char *temp = (char*)malloc(length*sizeof(char));
    if(temp == NULL){
        return memory_error;
    }
    char *temp_str;
    char symbol = getchar();
    while(symbol != '\n' && symbol != EOF){
        temp[i] = symbol;
        i++;
        if (i >= length){
            length *= 2;
            temp_str = (char*)realloc(temp, length*sizeof(char));
            if(temp_str == NULL){
                free(temp);
                return memory_error;
            }
            temp = temp_str;
        }
        symbol = getchar();
    }
    temp[i] = '\0';
    str->length = strlen(temp);
    str->string = (char*)malloc((str->length + 1)*sizeof(char));
    if(str->string == NULL){
        return memory_error;
    }
    strcpy(str->string, temp);
    free(temp);

    if(flag == 2 && is_valid_index(str) == fail){
        return wrong_input;
    }
    else if(flag == 3 && is_valid_id(str) == fail){
        return wrong_input;
    }
    else if(flag == 4 && is_valid_time(str) == fail){
        return wrong_input;
    }
    return success;
}

int is_valid_time(const String *str){
    if(str->length != 19){
        return fail;
    }
    for (int i = 0; i < 19; i++){
        if (str->string[i] == ' ' && i != 10){
            return fail;
        }
        if((str->string[i] > '3' || !isdigit(str->string[i])) && i == 0){
            return fail;
        }
        if((str->string[i] > '1' || !isdigit(str->string[i])) && i == 3){
            return fail;
        }
        if(((str->string[i-1] == '1' && str->string[i] > '2') || !isdigit(str->string[i])) && i == 4){
            return fail;
        }
        if((str->string[i] > '2' || !isdigit(str->string[i])) && i == 11){
            return fail;
        }
        if((str->string[i] > '5' || !isdigit(str->string[i])) && (i == 14 || i == 17)){
            return fail;
        }
        if(i == 2 || i == 5 || i == 13 || i == 16){
            if(str->string[i] != ':'){
                return fail;
            }
        }
        else if (!isdigit(str->string[i]) && i != 10){
            return fail;
        }
    }
    return success;
}

int is_valid_id(const String *str){
    if(str->length != 14){
        return fail;
    }
    for(int i = 0; i < 14; i++){
        if(!isdigit(str->string[i])){
            return fail;
        }
    }
    return success;
}

int is_valid_index(const String *str){
    if(str->length != 6){
        return fail;
    }
    for(int i = 0; i < 6; i++){
        if(!isdigit(str->string[i])){
            return fail;
        }
    }
    return success;
}

void delete(String *str){
    if(str->string != NULL){
        free(str->string);
        str->string = NULL;
    }
}

void delete_packages(Post *post){
    if (post->packages != NULL){
        for (int i = 0; i < post->packages_number; i++){
            delete(&post->packages[i].address_client.city);
            delete(&post->packages[i].address_client.street);
            delete(&post->packages[i].address_client.building);
            delete(&post->packages[i].address_client.index);
            delete(&post->packages[i].id);
            delete(&post->packages[i].creation_time);
            delete(&post->packages[i].delivery_time);
        }
        free(post->packages);
        post->packages = NULL;
        post->packages_number = 0;  
    }
}

void print(int state){
    if(state == menu){
        printf("\tMAIN MENU\nChoose option:\n 1. Add package\n 2. Delete package\n 3. Info about package\n 4. All delivered packages\n 5. All packages\n 6. Exit\n Enter a number: ");
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
        printf("Choose option:\n 1. Back to menu\n 2. Exit\n Enter a number: ");
    }
    else if(state == id_error){
        printf("This id is already taken.\n");
    }
    else if(state == fail_time){
        printf("Time is wrong.\n");
    }
}