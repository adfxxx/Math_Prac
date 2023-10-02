#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define pi 3.14

enum errors{
    wrong_amount = -1,
    wrong_eps = -2
};

int eps_check (char *number){
    int length = strlen(number);
    int one_count = 0;
    int dot_count = 0;
    for (int i = 0; i < length; ++i){
        if (number[i] == '0' || number[i] == '1' || number[i] == '.'){
            if (number[i] == '1'){
                one_count += 1;
            }
            if (number[i] == '.'){
                dot_count += 1;
            }
        }
        else{ return 0; }
    }
    if (one_count != 1 || dot_count != 1){
        return 0;
    }
    return 1;
}

double function_a(double x){
    return (log(1+x)/x);
}

double integral_a(double eps){
    int a = 0;
    int b = 1;
    double h = (b-a)*eps;
    double x = a + h;
    double total = 0;
    while(x < b){
        total += 4*function_a(x);
        x += h;
        if (x >= b){ break; }
        total += 2*function_a(x);
        x += h;
    }
    return (h/3)*(total + function_a(b));
}

double function_b(double x){
    return (exp(((-1.0)*x*x)/2));
}

double integral_b(double eps){
    int a = 0;
    int b = 1;
    double h = (b-a)*eps;
    double x = a + h;
    double total = 0;
    while(x < b){
        total += 4*function_b(x);
        x += h;
        if (x >= b){ break; }
        total += 2*function_b(x);
        x += h;
    }
    return (h/3)*(total + function_b(a) + function_b(b));
}

double function_c(double x){
    return(log(1/(1-x)));
}

double integral_c(double eps){
    int a = 0;
    int b = 1;
    double h = (b-a)*eps;
    double x = a + h;
    double total = 0;
    while(x < b){
        total += 4*function_c(x);
        x += h;
        if (x >= b){ break; }
        total += 2*function_c(x);
        x += h;
    }
    return (h/3)*(total);
}

double function_d(double x){
    return pow(x, x);
}

double integral_d(double eps){
    int a = 0;
    int b = 1;
    double h = (b-a)*eps;
    double x = a + h;
    double total = 0;
    while(x < b){
        total += 4*function_d(x);
        x += h;
        if (x >= b){ break; }
        total += 2*function_d(x);
        x += h;
    }
    return (h/3)*(total + function_d(a) + function_d(b));
}

int main (int argc, char *argv[]){
    if (argc != 2){
        printf("Wrong amount");
        return(wrong_amount);
    }
    if(!eps_check(argv[1])){
        printf ("Wrong eps");
        return (wrong_eps);
    }

    double eps = atof(argv[1]);

    printf("function a answer: %f\n", integral_a(eps));
    printf("function b answer: %f\n", integral_b(eps));
    printf("function c answer: %f\n", integral_c(eps));
    printf("function d answer: %f\n", integral_d(eps));
    
    return 0;
}