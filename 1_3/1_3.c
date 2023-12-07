#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int flag_check(char *flag){
    char flags [6][3] = {"-q", "/q", "-m", "/m", "-t", "/t"};
    
    if (strlen(flag) != 2){
        return 0;
    }

    for (int i = 0; i < 6; ++i){
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
        return 2;
    }
    return 1;
}

int flag_m (int number_1, int number_2) {
    if (number_1 == 0 || number_2 == 0){
        return 2;
    }
    if (number_1 % number_2 == 0) {
        return 1;
    }
    else return 0;
}

double find_max (double side_1, double side_2, double side_3){
    return ((side_1 > side_2 && side_1 > side_3) ? (side_1) : ((side_2 > side_3 && side_2 > side_1) ? (side_2) : (side_3)));
}

double find_min (double side_1, double side_2, double side_3){
    return ((side_1 < side_2 && side_1 < side_3) ? (side_1) : ((side_2 < side_3 && side_2 < side_1) ? (side_2) : (side_3)));
}

int flag_t(double eps, double side_1, double side_2, double side_3){
    double maxi = find_max(side_1, side_2, side_3);
    double mini = find_min(side_1, side_2, side_3);
    if (side_1 > eps && side_2 > eps && side_3 > eps){
        if (fabs((pow(maxi, 2) - (pow(mini, 2) + pow(side_1 + side_2 + side_3 - maxi - mini, 2)))) < eps){
            return 1;
        }
        else { return 0;}
    }
    else {return 0; }
}

int combination (double *numbers){
    int middle = 1;
    while (middle != -1 && numbers[middle] >= numbers[middle+1]) {
        middle--;
    }
    if (middle == -1){
        return 0;
    }
    int last = 2;
    while (numbers [middle] >= numbers[last]){
        last--;
    }

    double tmp = numbers[middle];
    numbers[middle] = numbers[last];
    numbers[last] = tmp;

    int new_last = middle + 1;
    int new_middle = 2;
    while (new_last < new_middle){
        double tmp = numbers[new_last];
        numbers[new_last] = numbers[new_middle];
        numbers[new_middle] = tmp;

        new_last++;
        new_middle--;
    }
    return 1;
}

double flag_q (double eps, double a, double b, double c){
    double discr = sqrt(b*b - 4*a*c);
    if (discr > eps){
        double x_1 = (-b + discr)/(2*a);
        double x_2 = (-b - discr)/(2*a);
        printf("For coefs %.3f %.3f %.3f x1 = %f, x2 = %f\n", a, b, c, x_1, x_2);
    }
    else if (discr == 0){
        printf("For coefs %.3f %.3f %.3f x = \n", a, b, c, ((-b)/(2*a)));
    }
    else { printf ("For coefs %.3f %.3f %.3f are no x.\n", a, b, c);}
}

void sort_numbers(double *numbers)
{
    double mini = find_min(numbers[0], numbers[1], numbers[2]);
    double maxi = find_max(numbers[0], numbers[1], numbers[2]);
    double middle = numbers[0] + numbers[1] + numbers[2] - mini - maxi;
    numbers[0] = mini;
    numbers[1] = middle;
    numbers[2] = maxi;
    return;
}

int main(int argc, char *argv[]){
    if (!flag_check(argv[1])){
        printf ("Wrong flag.\n");
        return 0;
    }

    char flag = argv[1][1];

    switch(flag){
        case 'q':
            if (argc != 6){
                printf("Not enough args.\n");
                return 0;
            }
            if (!number_check(argv[3]) ||  !number_check(argv[4]) || !number_check(argv[5]) || number_check(argv[2]) == 2 || !number_check(argv[2])){
                printf ("Wrong numbers.\n");
                return 0;
            }
            if (argv[2][0] == '-'){
                printf ("Wrong eps.\n");
                return 0;
            }
            double point_q = atof(argv[2]);
            double eps_1 = pow(10, -point_q);
            double a = atof(argv[3]);
            double b = atof(argv[4]);
            double c = atof(argv[5]);
            double numbers[]= {a, b, c};
            sort_numbers(numbers);
            do {
                flag_q(eps_1, numbers[0], numbers[1], numbers[2]);
            } while (combination(numbers));
            break;
        case 'm':
            if (argc != 4){
                printf("Wrong number of args.\n");
                return 0;
            }
            if (number_check(argv[2]) == 0 ||  number_check(argv[3]) == 0){
                printf ("Wrong numbers.\n");
                return 0;
            }
            if (number_check(argv[2]) == 2 ||  number_check(argv[3]) == 2){
                printf ("Wrong numbers.\n");
                return 0;
            }
            int number_1 = atoi(argv[2]);
            int number_2 = atoi(argv[3]);
            if (flag_m(number_1, number_2) == 2) { printf ("Wrong numbers.\n");}
            else if (!flag_m(number_1, number_2)){
                printf ("First number is not multiple of the second.\n");
            }
            else { 
                printf("First number is multiple of the second.\n");
            }
            break;
        case 't':
            if (argc != 6){
                printf("Wrong number of args.\n");
                return 0;
            }
            if (!number_check(argv[3]) ||  !number_check(argv[4]) || !number_check(argv[5]) || number_check(argv[2]) == 2 || !number_check(argv[2])){
                printf ("Wrong numbers.\n");
                return 0;
            }
            if (argv[2][0] == '-' || argv[3][0] == '-' || argv[4][0] == '-' || argv[5][0] == '-'){
                printf ("Wrong numbers.\n");
                return 0;
            }
            double point_t = atof(argv[2]);
            double eps_2 = pow(10, -point_t);
            double side_1 = atof(argv[3]);
            double side_2 = atof(argv[4]);
            double side_3 = atof(argv[5]);
            if (!flag_t(eps_2, side_1, side_2, side_3)){
                printf ("Numbers are not sides of a triangle.\n");
            }
            else {printf ("Numbers are sides of a triangle.\n");}
            break;
    }
    return 0;
}