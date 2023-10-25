#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_input = -5,
    memory_error = -4,
    wrong_input_eps = -3,
    wrong_input_border = -2    
};

double check_border(char **num);
double check_eps (char **eps);
double function_1(double x);
double function_2(double x);
double function_3(double x);
double function_4(double x);
double dichotomy(char *a_str, char *b_str, char *eps_str, double func(double));

int main(){
    char a_str[256];
    char b_str[256];
    char eps_str[256];
    printf("Enter left border: ");
    if(fgets(a_str, sizeof(a_str), stdin) == NULL){
        printf("Input error");
        return wrong_input;
    }
    printf("Enter right border: ");
    if(fgets(b_str, sizeof(b_str), stdin) == NULL){
        printf("Input error");
        return wrong_input;
    }
    printf("Enter epsilon: ");
    if(fgets(eps_str, sizeof(a_str), stdin) == NULL){
        printf("Input error");
        return wrong_input;
    } 
    double result_1 = dichotomy(a_str, b_str, eps_str, function_1);
    double result_2 = dichotomy(a_str, b_str, eps_str, function_2);
    double result_3 = dichotomy(a_str, b_str, eps_str, function_3);
    double result_4 = dichotomy(a_str, b_str, eps_str, function_4);
    if(result_1 == wrong_input_eps || result_2 == wrong_input_eps || result_3 == wrong_input_eps || result_4 == wrong_input_eps){
        printf("Wrong eps for function");
        return wrong_input;
    }    
    else if(result_1 == wrong_input_border || result_2 == wrong_input_border || result_3 == wrong_input_border || result_4 == wrong_input_border){
        printf("Wrong border for function");
        return wrong_input;
    }         
    else{
        printf("Result for function 'exp(x) + sqrt(1+exp(2*x))-2' [a = %s, b = %s, eps = %s]: %.4lf\n", a_str, b_str, eps_str, result_1);
        printf("Result for function 'exp(x) + sqrt(1+exp(2*x))-2' [a = %s, b = %s, eps = %s]: %.4lf\n", a_str, b_str, eps_str, result_2);
        printf("Result for function 'x^3 - 3*x + 1' [a = %s, b = %s, eps = %s]: %.4lf\n", a_str, b_str, eps_str, result_3);
        printf("Result for function '5*cos(x)-log(x)-1' [a = %s, b = %s, eps = %s]: %.4lf\n", a_str, b_str, eps_str, result_4);
    }
    return success;
}

double check_border(char **num){
    if ((*num)[0] == '\0'){
        return wrong_input;
    }
    int length = strlen(*num);
    if ((*num)[length - 1] == '\n'){
        (*num)[length - 1] = '\0';
    }
    for (int i = 0; i < length; i++){
        if ((*num)[i] == '\0'){
            break;
        }
        if (!isdigit((*num)[i]) && (*num)[i] != '-' && (*num)[i] != '.' && (*num)[i] != '\n'){
            return wrong_input;
        }
    }
    double result = atof(*num);
    return result;
}

double check_eps (char **eps){
    if ((*eps)[0] == '\0'){
        return wrong_input;
    }
    int length = strlen(*eps);
    if ((*eps)[length - 1] == '\n') {
        (*eps)[length - 1] = '\0';
    }
    int dot_count = 0;
    for (int i = 0; i < length; i++){
        if ((*eps)[i] == '\0'){
            break;
        }
        else if ((*eps)[i] != '0' && (*eps)[i] != '1' && (*eps)[i] != '.' && (*eps)[i] != '\n'){
            return wrong_input;
        }
        else if ((*eps)[i] == '.'){
            dot_count += 1;
        }
        else if (dot_count > 1){
            return wrong_input;
        }
    }
    if(dot_count != 1){
        return wrong_input;
    }
    double result = atof(*eps);
    return result;
}

// a = -1 b = 0
double function_1(double x){
    return exp(x) + sqrt(1+exp(2*x))-2; 
}

// a = 0.4 b = 1.0 
double function_2(double x){
    return 2*x*sin(x) - cos(x); 
}

// a = 0 b = 1
double function_3(double x){
    return pow(x, 3) - 3*x + 1; 
}

// a = 1 b = 2
double function_4(double x){
    return 5*cos(x)-log(x)-1; 
}

double dichotomy(char *a_str, char *b_str, char *eps_str, double func(double)){
    double eps;
    double a;
    double b;
    if(check_eps(&eps_str) == wrong_input){
        return wrong_input_eps;
    }
    else{
        eps = check_eps(&eps_str);
    }
    if(check_border(&a_str) == wrong_input || check_border(&b_str) == wrong_input){
        return wrong_input_border;
    }
    else{
        a = check_border(&a_str);
        b = check_border(&b_str);
    }
    double funct = b;
    double x = (a + b)/2;
    while (fabs(funct - x) > eps){
        if (func(x)*func(a) > 0){
            a = x;
        }
        else{
            b = x;
        }
        funct = x;
        x = (a + b)/2;
    } 
    return x;
}