#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int flag_check(char *flag){
    char flags [12][3] = {"-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f"};
    
    if (strlen(flag) != 2){
        return 0;
    }

    for (int i = 0; i < 12; ++i){
        if (strcmp(flag, flags[i]) == 0){
            return 1;
        }
    }

    return 0;
}

int number_check(char *number){
    int length = strlen(number);
    int dot_count = 0;
    for (int i = 0; i < length; ++i){
        if ((number[i] >= '0' && number[i] <= '9') || number[i] == '.' || number[i] == '-'){
            if (number[i] == '.'){
                dot_count += 1;
            }
        }
        else{ return 0; }
    }
    if (dot_count != 0){
        return 0;
    }
    return 1;
}

int flag_h(int number){
    int status = 0;
    for (int i = number; i <= 100; ++i){
        if (i % number == 0){
            printf("%d ", i);
            status = 1;
        }
    }
    if (status == 0){ printf("There are no numbers");} 
}

int flag_p(int number){
    if (number >= 2){
        for (int i = 2; i < number; ++i ){
            if (number % i == 0){
                return 0;
            }
        }
        return 1;
    }
    else if (number == 1){
        return 0;
    }
    else { return -1; }
}

int flag_s(char *number){
    int position = 0;
    if (number[0] == '-'){
        position += 1;
    }
    if (number[position] == '0'){
        while (number[position] == '0'){
            position += 1;
        }
    }
    for (int i = position; i < strlen(number); ++i){
        printf("%c ", number[i]);
    }
}

long long int calculations_for_e(int num, int pow){
    long long int total = num;
    for (int i = 1; i < pow; ++i){
        total *= num;
    }
    return total;
}

int flag_e(int number){
    printf("|   Power   |     Number    |\n");
    printf("|-----------|---------------|\n");
    for (int i = 1; i <= number; ++i){
        printf ("|%6d     |", i);
        for (int j = 1; j <= 10; ++j){
            printf("%11lli    |\n", calculations_for_e(j, i));
            if (j != 10){
                printf("|           |");
            }
        }
        printf("|___________|_______________|\n");
    }
}

int flag_a (int number) {
    int result = 0;
    if (number > 0){
        result = (number*(number+1))/2;
        return result;
    }
    if (number <= 0){
        return 0;
    }
}

long long int flag_f(long long int number){
    long long int total = 1;
    for (int i = 2; i <= number; ++i){
        total *= i;
    }
    return total;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf ("You must enter 2 arguments.\n");
        return 1;
    }

    if (!number_check(argv[1])){
        printf ("Wrong number.\n");
        return 0;
    }

    if (!flag_check(argv[2])){
        printf ("¬веден неправильный флаг.\n");
        return 0;
    }

    int number = atoi(argv[1]);
    char flag = argv[2][1];

    switch (flag){
        case 'h':
            if (number <= 0 || number > 100){
                printf("Number should be above 0 and below 100\n");
            }
            else{
                flag_h(number);
            }
            break;
        case 'p':
            if(flag_p(number) == 1){
                printf("Number is prime.\n");
            }
            else if (!flag_p(number)){
                printf("Number is not prime.\n");
            }
            else {printf ("Number must be above zero.\n");}
            break;
        case 's':
            flag_s(argv[1]);
            break;
        case 'e':
            if (number <= 0 || number > 10) {
                printf ("Number must be above 0 and below or equal 10");
            }
            else{
                flag_e(number);
            }
            break;
        case 'a':
            if (!flag_a(number)){
                printf("Number must be above zero.");
            }
            else {printf("%d", flag_a(number));}
            break;
        case 'f':
            if(number > 20 || number < 0){
                printf("Number must be above zero and below 20.");
            }
            else if (number == 0){
                printf("%d", 1);
            }
            else {printf("%lli", flag_f(number));}
            break;
    }
    return 0;
}