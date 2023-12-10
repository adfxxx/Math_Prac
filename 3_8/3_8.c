#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

enum errors{
    success = 1,
    wrong_input = -1,
    memory_error = -2,
    is_not_open_input = -3,
    fail = -4
};

enum commands{
    add = 2,
    sub = 3,
    mult = 4,
    Div = 5,
    mod = 6,
    eval = 7,
    diff = 8,
    cmps = 9
};

typedef struct Monom{
    int coef;
    int power;
    struct Monom *next;
}Monom;

typedef struct Polynom{
    Monom *head;
    struct Polynom *next;
}Polynom;

int read_string(char *line, Polynom **polynoms, Polynom *summator, int *check_sqbr);
int check_string(char *line, int *check_sqbr);
int get_polynom(char *line, Polynom *poly);
void reverse(char *line);
int add_monom(int coef, int power, Polynom *poly);
void create_polynom(Polynom *poly);
Polynom copy_poly(const Polynom *poly);
void free_polynom(Polynom *poly);
int command_check(const char *command);
int change_command(int command, Polynom **polynoms, Polynom *summator, int count);
int add_poly(Polynom *poly_1, Polynom *poly_2, Polynom **temp);
int sub_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer);
int mult_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer);
void combine_monoms(Polynom *poly);
void sort_poly(Polynom *poly);
int div_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer);
int mod_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer);
int eval_poly(Polynom *poly_1, Polynom *poly_2);
int diff_poly(Polynom *poly, Polynom **answer);
int cmps_poly(Polynom *poly_1, Polynom *poly_2, Polynom **temp);
void print_poly(Polynom *poly);
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 2){
        print(wrong_input);
        return wrong_input;
    }
    FILE *input = fopen(argv[1], "r");
    if(!input){
        print(is_not_open_input);
        return is_not_open_input;
    }

    Polynom *polynoms = NULL;
    char *line = NULL;
    size_t length = 0;
    Polynom summator;
    int result;
    int check_sqbr = 0;
    create_polynom(&summator);
    result = add_monom(0, 0, &summator);
    if(result == memory_error){
        print(memory_error);
        fclose(input);
        return memory_error;
    }
    while(getline(&line, &length, input) != -1){
        result = read_string(line, &polynoms, &summator, &check_sqbr);
        if(result == memory_error){
            print(memory_error);
        }
    }
    free_polynom(&summator);
    free(line);
    fclose(input);
    return success;
}

int read_string(char *line, Polynom **polynoms, Polynom *summator, int *check_sqbr){
    int count = 0;
    if(check_string(line, check_sqbr) == wrong_input){
        return wrong_input;
    }
    char *token = strtok(line, "(");
    char *command_str = NULL;
    char *polynom;
    int command = -1;
    int result;
    while(token != NULL){
        if(command_str == NULL){
            command_str = token;
            token = strtok(NULL, "(");
        }
        char *command_end = strchr(token, ')');
        if(command_end != NULL){
            polynom = strtok(token, ",");
            command = command_check(command_str);
            if(command == fail){
                free(token);
                free(polynom);
                free(command_str);
                return wrong_input;
            }
            Polynom *poly = (Polynom *)malloc(sizeof(Polynom));
            if(poly == NULL){
                free(token);
                free(polynom);
                free(command_str);
                return memory_error;
            }
            create_polynom(poly);
            result = get_polynom(polynom, poly);
            if(result == memory_error){
                free(token);
                free(polynom);
                free(command_str);
                free(poly);
                return memory_error;
            }
            if(poly->head != NULL){
                poly->next = *polynoms;
                *polynoms = poly;
                count += 1;
            }
            token = strtok(NULL, ",");
        }
    }
    result = change_command(command, polynoms, summator, count);
    if(result == memory_error){
        return memory_error;
    }
    return success;
}

int check_string(char *line, int *check_sqbr){
    int length = strlen(line);
    int check_br = 0;
    int check_sc = 0;
    int flag = 0;
    for(int i = 0; i < length; i++){
        if(line[i] == '('){
            check_br++;
            flag = 1;
        }
        else if(line[i] == ')'){
            check_br--;
        }
        else if(line[i] == ';'){
            check_sc++;
        }
        else if(line[i] == '%'){
            break;
        }
        else if(line[i] == '['){
            if(*check_sqbr != 0){
                return wrong_input;
            }
            else{
                (*check_sqbr)++;
                flag = 2;
            }
        }
        else if(line[i] == ']'){
            if(*check_sqbr == 0){
                return wrong_input;
            }
            else{
                (*check_sqbr)--;
                flag = 2;
            }
        }
    }
    if(check_br == 0 && check_sc == 1 && ((flag == 1 && *check_sqbr == 0) || (flag == 2 && *check_sqbr != 0))){
        return success;
    }
    else{
        return wrong_input;
    }
}

int get_polynom(char *line, Polynom *poly){
    char *end = strchr(line, ')');
    if(end != NULL){
        *end = '\0';
    }
    int coef = 0;
    int power = 0;
    int result;
    int length = strlen(line);
    int flag = 0;

    char *coef_str = (char*)malloc(2*sizeof(char));
    if(coef_str == NULL){
        return memory_error;
    }
    char *power_str = (char*)malloc(2*sizeof(char));
    if(power_str == NULL){
        free(coef_str);
        return memory_error;
    }
    int j = 0;
    int k = 0;
    char *temp = NULL;
    for(int i = length-1; i >= 0; i--){
        if(line[i] != 'x' && (flag == 0 || flag == 2) && line[i] != ' ' && line[i] != '-' && line[i] != '+'){
            if(j > 0){
                temp = (char*)realloc(power_str, (j+1)*sizeof(char));
                if(temp == NULL){
                    free(coef_str);
                    free(power_str);
                    return memory_error;
                }
                power_str = temp;
            }
            power_str[j] = line[i];
            j++;
            flag = 2;
        }
        if(line[i] == 'x'){
            if(flag != 3){
                power = 1;
            }
            else{
                power_str[j] = '\0';
                if(j >= 2){
                    reverse(power_str);
                }
                power = atoi(power_str);
            }
            flag = 1;
        }
        if(line[i] == '^'){
            flag = 3;
        }
        if(line[i] != 'x' && (flag == 1 || flag == 3) && line[i] != '-' && line[i] != '+' && line[i] != ' ' && line[i] != '^'){
            if(k > 0){
                temp = (char*)realloc(coef_str, (k+1)*sizeof(char));
                if(temp == NULL){
                    free(coef_str);
                    free(power_str);
                    return memory_error;
                }
                coef_str = temp;
            }
            coef_str[k] = line[i];
            k++;
            flag = 3;
        }
        if((line[i] == '-' || line[i] == '+') && i != 0){
            if(flag == 1){
                coef = 1;
            }
            else if(flag == 2){
                power_str[j] = '\0';
                if(j >= 2){
                    reverse(power_str);
                }
                coef = atoi(power_str);
            }
            else{
                coef_str[k] = '\0';
                if(k >= 2){
                    reverse(coef_str);
                }
                coef = atoi(coef_str);
            }
            if(line[i] == '-'){
                coef *= -1;
            }
            result = add_monom(coef, power, poly);
            if(result == memory_error){
                free(coef_str);
                free(power_str);
                return memory_error;
            }
            free(coef_str);
            free(power_str);
            coef_str = (char*)malloc(sizeof(char));
            if(coef_str == NULL){
                return memory_error;
            }
            power_str = (char*)malloc(sizeof(char));
            if(power_str == NULL){
                free(coef_str);
                return memory_error;
            }
            power = 0;
            coef = 0;
            flag = 0;
            j = 0;
            k = 0;
            if(i == 1 && line[i-1] == ' '){
                break;
            }
        }
        if(i == 0){
            if(flag == 3){
                coef_str[k] = '\0';
                if(k >= 2){
                    reverse(coef_str);
                }
                coef = atoi(coef_str);
            }
            else if(flag == 1 || line[i] == ' '){
                coef = 1;
            }
            else if(flag == 2){
                power_str[j] = '\0';
                if(j >= 2){
                    reverse(power_str);
                }
                coef = atoi(power_str);
            }
            if(line[i] == '-'){
                coef *= -1;
            }
            result = add_monom(coef, power, poly);
            if(result == memory_error){
                free(coef_str);
                free(power_str);
                return memory_error;
            }
        }
    }
    sort_poly(poly);
    free(coef_str);
    free(power_str);
    return success;
}

void reverse(char *line){
    int len = strlen(line);
    if(line[len-1] == '^'){
        len--;
    }
    char symbol;
    int i = 0;
    int j = len - 1;
    while(i < j){
        symbol = line[i];
        line[i] = line[j];
        line[j] = symbol;
        i++;
        j--;
    }
}

int change_command(int command, Polynom **polynoms, Polynom *summator, int count){
    Polynom *temp;
    Polynom save;
    int result = 0;
    switch(command){
        case add:
        if(*polynoms != NULL){
            if(count == 1){
                result = add_poly(*polynoms, summator, &temp);
            }
            else{
                result = add_poly(*polynoms, (*polynoms)->next, &temp);
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            (*summator) = copy_poly(temp);
            sort_poly(summator);
            printf("Result of add: ");
            print_poly(summator);
            printf("\n");
            save = copy_poly(summator);
            free_polynom(*polynoms);
            (*summator) = save;
        }
        else{
            printf("There are no polynoms to add.\n");
        }
        break;
        case sub:
        if(*polynoms != NULL){
            if(count == 1){
                result = sub_poly(summator, *polynoms, &temp);
            }
            else{
                result = sub_poly((*polynoms)->next, *polynoms, &temp);
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            (*summator) = *temp;
            sort_poly(summator);
            printf("Result of sub: ");
            print_poly(summator);
            printf("\n");
            save = copy_poly(summator);
            free_polynom(*polynoms);
            (*summator) = save;
        }
        else{
            printf("There are no polynoms to sub.\n");
        }
        break;
        case mult:
        if(*polynoms != NULL){
            if(count == 1){
                result = mult_poly(*polynoms, summator, &temp);
            }
            else{
                result = mult_poly(*polynoms, (*polynoms)->next, &temp);
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            (*summator) = copy_poly(temp);
            sort_poly(summator);
            printf("Result of mult: ");
            print_poly(summator);
            printf("\n");
            save = copy_poly(summator);
            free_polynom(*polynoms);
            (*summator) = save;
        }
        else{
            printf("There are no polynoms to mult.\n");
        }
        break;
        case Div:
        if(*polynoms != NULL){
            if(count == 1){
                if(summator->head->coef == 0 && summator->head->power == 0){
                    printf("Result of div: 0\n");
                }
                else{
                    result = div_poly(summator, *polynoms, &temp);
                }
            }
            else{
                if((*polynoms)->head == NULL || (*polynoms)->head->coef == 0){
                    printf("Result of div: wrong input - division by zero.");
                }
                else{
                    result = div_poly((*polynoms)->next, *polynoms, &temp);
                }
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            else if(result == success){
                (*summator) = *temp;
                sort_poly(summator);
                printf("Result of div: ");
                print_poly(summator);
                printf("\n");
                save = copy_poly(summator);
                free_polynom(*polynoms);
                (*summator) = save;
            }
        }
        else{
            printf("There are no polynoms to div.\n");
        }
        break;
        case mod:
        if(*polynoms != NULL){
            if(count == 1){
                if(summator->head->coef == 0 && summator->head->power == 0){
                    printf("Result of mod: 0\n");
                }
                else{
                    result = mod_poly(summator, *polynoms, &temp);
                }
            }
            else{
                if((*polynoms)->head == NULL || (*polynoms)->head->coef == 0){
                    printf("Result of mod: wrong input - division by zero.");
                }
                else{
                    result = mod_poly((*polynoms)->next, *polynoms, &temp);
                }
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            else if(result == success){
                (*summator) = *temp;
                sort_poly(summator);
                printf("Result of mod: ");
                print_poly(summator);
                printf("\n");
                save = copy_poly(summator);
                free_polynom(*polynoms);
                (*summator) = save;
            }
        }
        else{
            printf("There are no polynoms to mod.\n");
        }
        break;
        case eval:
        if(*polynoms != NULL){
            if(count == 1){
                result = eval_poly(summator, *polynoms);
                printf("Result of eval: ");
                printf("%d\n", result);
            }
            else{
                result = eval_poly((*polynoms)->next, *polynoms);
                printf("Result of eval: ");
                printf("%d\n", result);
                free_polynom(*polynoms);
                summator->head->coef = result;
            }
        }
        else{
            printf("There are no polynoms to mod.\n");
        }
        break;
        case diff:
        if(*polynoms != NULL){
            if(count == 0){
                result = diff_poly(summator, &temp);
            }
            else if(count == 1){
                result = diff_poly(*polynoms, &temp);
            }
            else{
                printf("Too many polinoms for fuction");
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            (*summator) = *temp;
            sort_poly(summator);
            printf("Result of diff: ");
            print_poly(summator);
            printf("\n");
            save = copy_poly(summator);
            free_polynom(*polynoms);
            (*summator) = save;
        }
        else{
            printf("There are no polynoms to mod.\n");
        }
        break;
        case cmps:
        if(*polynoms != NULL){
            if(count == 1){
                result = cmps_poly(summator, *polynoms, &temp);
            }
            else{
                result = cmps_poly((*polynoms)->next, *polynoms, &temp);
            }
            if(result == memory_error){
                free_polynom(summator);
                free_polynom(*polynoms);
                return memory_error;
            }
            (*summator) = *temp;
            sort_poly(summator);
            printf("Result of cmps: ");
            print_poly(summator);
            printf("\n");
            save = copy_poly(summator);
            free_polynom(*polynoms);
            (*summator) = save;
        }
        else{
            printf("There are no polynoms to cmps.\n");
        }
        break;
    }
}

int cmps_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer){
    int check;
    sort_poly(poly_1);
    Polynom result;
    create_polynom(&result);
    Polynom temp;
    Polynom *temp_2;
    create_polynom(&temp);
    check = add_monom(1, 0, &temp);
    if(check == memory_error){
        return memory_error;
    }
    Monom *cur_1 = poly_1->head;
    Polynom temp_result;
    Polynom coef;
    create_polynom(&coef);
    while(cur_1 != NULL){
        for(int i = 0; i < cur_1->power; i++){
            check = mult_poly(poly_2, &temp, &temp_2);
            if(check == memory_error){
                free_polynom(&coef);
                free_polynom(&temp);
                free_polynom(&result);
                return memory_error;
            }
            temp = copy_poly(temp_2);
        }
        if(cur_1->power == 1){
            temp = *poly_2;
        }
        else if(cur_1->power == 0){
            free_polynom(&temp);
            check = add_monom(cur_1->coef, 0, &temp);
            if(check == memory_error){
                return memory_error;
            }
        }
        if(cur_1->power != 0){
            check = add_monom(cur_1->coef, 0, &coef);
            if(check == memory_error){
                return memory_error;
            }
            check = mult_poly(&temp, &coef, &temp_2);
            if(check == memory_error){
                free_polynom(&coef);
                free_polynom(&temp);
                free_polynom(&result);
                return memory_error;
            }
            temp_result = copy_poly(temp_2);
            temp = copy_poly(&temp_result);
            free_polynom(&temp_result);
        }
        temp_result = copy_poly(&result);
        free_polynom(&result);
        check = add_poly(&temp, &temp_result, &temp_2);
        if(check == memory_error){
            return memory_error;
        }
        result = copy_poly(temp_2);
        sort_poly(&result);
        free_polynom(&coef);
        free_polynom(&temp);
        check = add_monom(1, 0, &temp);
        if(check == memory_error){
            return memory_error;
        }
        free_polynom(&temp_result);
        cur_1 = cur_1->next;
    }
    combine_monoms(&result);
    (*answer) = &result;
    return success;
}

int diff_poly(Polynom *poly, Polynom **answer){
    Polynom result;
    int check;
    create_polynom(&result);
    if(poly->head->coef == 0 && poly->head->power == 0){
        check = add_monom(0, 0, &result);
        if(check == memory_error){
            return memory_error;
        }
        (*answer) = &result;
        return success;
    }
    Monom *cur = poly->head;
    int new_coef = 0;
    int new_power = 0; 
    while(cur != NULL){
        new_coef = cur->coef*cur->power;
        new_power = cur->power - 1;
        check = add_monom(new_coef, new_power, &result);
        if(check == memory_error){
            free_polynom(&result);
            return memory_error;
        }
        cur = cur->next;
    }
    (*answer) = &result;
    return success;
}

int eval_poly(Polynom *poly_1, Polynom *poly_2){
    int result = 0;
    Monom *cur_1 = poly_1->head;
    int point = poly_2->head->coef;
    while(cur_1 != NULL){
        result += cur_1->coef*pow(point, cur_1->power);
        cur_1 = cur_1->next;
    }
    return result;
}

int mod_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer){
    if(poly_1->head->power < poly_2->head->power){
        (*answer) = poly_1;
        return fail;
    }
    Polynom temp;
    Polynom temp_product;
    Polynom temp_sub;
    Polynom *temp_2;
    int coef = 0;
    int power = 0;
    int check;
    while (poly_1->head != NULL && poly_1->head->power >= poly_2->head->power) {
        coef = poly_1->head->coef / poly_2->head->coef;
        power = poly_1->head->power - poly_2->head->power;

        create_polynom(&temp);
        check = add_monom(coef, power, &temp);
        if(check == memory_error){
            return memory_error;
        }

        create_polynom(&temp_product);
        check = mult_poly(poly_2, &temp, &temp_2);
        if(check == memory_error){
            free_polynom(&temp_product);
            free_polynom(&temp);
            free_polynom(&temp_sub);
            return memory_error;
        }
        temp_product = *temp_2;
        sort_poly(&temp_product);
 
        create_polynom(&temp_sub);
        check = sub_poly(poly_1, &temp_product, &temp_2);
        if(check == memory_error){
            free_polynom(&temp_product);
            free_polynom(&temp);
            free_polynom(&temp_sub);
            return memory_error;
        }
        temp_sub = *temp_2;
        free_polynom(poly_1);
        *poly_1 = copy_poly(&temp_sub);
        sort_poly(poly_1);
        free_polynom(&temp_product);
        free_polynom(&temp);
    }
    combine_monoms(poly_1);
    (*answer) = poly_1;
    return success;
}

int div_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer){
    Polynom result;
    create_polynom(&result);
    if(poly_1->head->power < poly_2->head->power){
        return fail;
    }
    Polynom temp;
    Polynom temp_product;
    Polynom temp_sub;
    Polynom *temp_2;
    int coef = 0;
    int power = 0;
    int check;
    while (poly_1->head != NULL && poly_1->head->power >= poly_2->head->power){
        if(poly_1->head->power - poly_2->head->power < 0){
            break;
        }
        coef = poly_1->head->coef / poly_2->head->coef;
        power = poly_1->head->power - poly_2->head->power;
        
        create_polynom(&temp);
        check = add_monom(coef, power, &temp);
        if(check == memory_error){
            free_polynom(&temp_product);
            free_polynom(&temp);
            free_polynom(&temp_sub);
            free_polynom(&result);
            return memory_error;
        }
        create_polynom(&temp_product);
        check = mult_poly(poly_2, &temp, &temp_2);
        if(check == memory_error){
            free_polynom(&temp_product);
            free_polynom(&temp);
            free_polynom(&temp_sub);
            free_polynom(&result);
            return memory_error;
        }
        temp_product = *temp_2;
        sort_poly(&temp_product);
        create_polynom(&temp_sub);
        check = sub_poly(poly_1, &temp_product, &temp_2);
        if(check == memory_error){
            free_polynom(&temp_product);
            free_polynom(&temp);
            free_polynom(&temp_sub);
            free_polynom(&result);
            return memory_error;
        }
        temp_sub = *temp_2;
        add_monom(coef, power, &result);

        *poly_1 = copy_poly(&temp_sub);
        sort_poly(poly_1);
        free_polynom(&temp_product);
        free_polynom(&temp);
    }
    free_polynom(&temp_sub);
    combine_monoms(&result);
    (*answer) = &result;
    return success;
}

void sort_poly(Polynom *poly){
    Monom *temp;
    Monom *cur;
    Monom *next;
    int temp_coef;
    int temp_power;
    for(cur = poly->head; cur != NULL; cur = cur->next){
        for(next = cur->next; next != NULL; next = next->next){
            if(cur->power < next->power){
                temp_coef = cur->coef;
                temp_power = cur->power;
                cur->coef = next->coef;
                cur->power = next->power;
                next->coef = temp_coef;
                next->power = temp_power;
            }
        }
    }
}

int mult_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer){
    Polynom result;
    create_polynom(&result);

    int new_coef;
    int new_power;
    int check;
    Monom *cur_1 = poly_1->head;
    while(cur_1 != NULL){
        Monom *cur_2 = poly_2->head;
        while(cur_2 != NULL){
            new_coef = cur_1->coef * cur_2->coef;
            new_power = cur_1->power + cur_2->power;
            check = add_monom(new_coef, new_power, &result);
            if(check == memory_error){
                return memory_error;
            }
            cur_2 = cur_2->next;
        }
        cur_1 = cur_1->next;
    }
    combine_monoms(&result);
    (*answer) = &result;
    return success;
}

void combine_monoms(Polynom *poly){
    Monom *cur = poly->head;
    while(cur != NULL){
        Monom *prev = cur;
        Monom *temp = cur->next;
        while(temp != NULL){
            if(cur->power == temp->power){
                cur->coef += temp->coef;
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
            else{
                prev = temp;
                temp = temp->next;
            }
        }
        cur = cur->next;
    }
}

int sub_poly(Polynom *poly_1, Polynom *poly_2, Polynom **answer){
    Polynom result;
    create_polynom(&result);

    Monom *cur_1 = poly_1->head;
    Monom *cur_2 = poly_2->head;
    int coef;
    int check;
    while(cur_1 != NULL && cur_2 != NULL){
        if(cur_1->power > cur_2->power){
            check = add_monom(cur_1->coef, cur_1->power, &result);
            if(check == memory_error){
                return memory_error;
            }
            cur_1 = cur_1->next;
        }
        else if(cur_1->power < cur_2->power){
            check = add_monom((-1)*cur_2->coef, cur_2->power, &result);
            if(check == memory_error){
                return memory_error;
            }
            cur_2 = cur_2->next;
        }
        else if(cur_1->power == cur_2->power){
            coef = cur_1->coef - cur_2->coef;
            if(coef != 0){
                check = add_monom(coef, cur_1->power, &result);
                if(check == memory_error){
                    return memory_error;
                }
            }
            cur_1 = cur_1->next;
            cur_2 = cur_2->next;
        }
    }
    while(cur_1 != NULL){
        check = add_monom(cur_1->coef, cur_1->power, &result);
        if(check == memory_error){
            return memory_error;
        }
        cur_1 = cur_1->next;
    }
    while(cur_2 != NULL){
        check = add_monom((-1)*cur_2->coef, cur_2->power, &result);
        if(check == memory_error){
            return memory_error;
        }
        cur_2 = cur_2->next;
    }
    (*answer) = &result;
    return success;
}

int add_poly(Polynom *poly_1, Polynom *poly_2, Polynom **temp){
    Polynom result;
    create_polynom(&result);

    Monom *cur_1 = poly_1->head;
    Monom *cur_2 = poly_2->head;
    int check;
    while(cur_1 != NULL && cur_2 != NULL){
        if(cur_1->power > cur_2->power){
            check = add_monom(cur_1->coef, cur_1->power, &result);
            if(check == memory_error){
                return memory_error;
            }
            cur_1 = cur_1->next;
        }
        else if(cur_1->power < cur_2->power){
            check = add_monom(cur_2->coef, cur_2->power, &result);
            if(check == memory_error){
                return memory_error;
            }
            cur_2 = cur_2->next;
        }
        else{
            check = add_monom(cur_1->coef + cur_2->coef, cur_1->power, &result);
            if(check == memory_error){
                return memory_error;
            }
            cur_1 = cur_1->next;
            cur_2 = cur_2->next;
        }
    }
    while (cur_1 != NULL) {
        check = add_monom(cur_1->coef, cur_1->power, &result);
        if(check == memory_error){
            return memory_error;
        }
        cur_1 = cur_1->next;
    }
    while (cur_2 != NULL) {
        check = add_monom(cur_2->coef, cur_2->power, &result);
        if(check == memory_error){
            return memory_error;
        }
        cur_2 = cur_2->next;
    }
    combine_monoms(&result);
    (*temp) = &result;
    return success;
}

void print_poly(Polynom *poly){
    Monom *cur = poly->head;
    while(cur != NULL){
        if(cur->coef == 0 && cur->power == 0){
            printf("0 ");
        }
        if(cur->coef == 1){
            if(cur->power == 0){
                printf("%d ", cur->coef);
            }
            else if(cur->power == 1){
                printf("x ");
            }
            else{
                printf("x^%d ", cur->power);
            }
        }
        else if(cur->coef != 0){
            if(cur->power == 1){
                printf("%dx ", cur->coef);
            }
            else if(cur->power == 0){
                printf("%d ", cur->coef);
            }
            else{
                printf("%dx^%d ", cur->coef, cur->power);
            }
        }
        cur = cur->next;
        if(cur != NULL && cur->coef != 0){
            printf("+ ");
        }
    }
}

Polynom copy_poly(const Polynom *poly){
    Polynom copy;
    create_polynom(&copy);

    Monom *cur = poly->head;
    while(cur != NULL){
        add_monom(cur->coef, cur->power, &copy);
        cur = cur->next;
    }
    return copy;
}

int add_monom(int coef, int power, Polynom *poly){
    Monom *new_monom = (Monom*)malloc(sizeof(Monom));
    if(new_monom == NULL){
        return memory_error;
    }
    new_monom->coef = coef;
    new_monom->power = power;
    new_monom->next = poly->head;
    poly->head = new_monom;

    return success;
}

void create_polynom(Polynom *poly){
    poly->head = NULL;
    poly->next = NULL;
}

void free_polynom(Polynom *poly){
    Monom *cur = poly->head;
    while(cur != NULL){
        Monom *temp = cur;
        cur = cur->next;
        free(temp);
    }
    poly->head = NULL;
    poly->next = NULL;
}

int command_check(const char *command){
    if(!strcmp(command, "Add")){
        return add;
    }
    if(!strcmp(command, "Sub")){
        return sub;
    }
    if(!strcmp(command, "Mult")){
        return mult;
    }
    if(!strcmp(command, "Div")){
        return Div;
    }
    if(!strcmp(command, "Mod")){
        return mod;
    }
    if(!strcmp(command, "Eval")){
        return eval;
    }
    if(!strcmp(command, "Diff")){
        return diff;
    }
    if(!strcmp(command, "Cmps")){
        return cmps;
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
}