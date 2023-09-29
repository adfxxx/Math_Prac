#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int number_check(char *number){
    int length = strlen(number);
    int dot_count = 0;
    for (int i = 0; i < length; ++i){
        if ((number[i] >= '0' && number[i] <= '9') || number[i] == '.' || number[i] == '-'){
            if (number[i] == '.'){
                dot_count += 1;
            }
            if (number[i] == '-'){
                return 2;
            }
        }
        else{ return 0; }
    }
    if (dot_count == 0 || dot_count != 1){
        return 2;
    }
    return 1;
}

int eps_check (char *number){
    int length = strlen(number);
    int one_count = 0;
    for (int i = 0; i < length; ++i){
        if (number[i] == '0' || number[i] == '1' || number[i] == '.'){
            if (number[i] == '1'){
                one_count += 1;
            }
        }
        else{ return 0; }
    }
    if (one_count != 1){
        return 0;
    }
    return 1;
}

double fac(int n) {
	if (n == 0) { return 1; }
	if (n > 0) { return n * fac(n - 1); }
}

double function_a(double eps, double x){
    int n = 0;
    double sum = 0;
    double element= 0;
    do{
        element = pow(x, n)/fac(n);
        sum += element;
        n++;
    } while(fabs(element) > eps);
    return sum;
}

double function_b(double eps, double x){
    int n = 0;
    double sum = 0;
    double element= 0;
    do{
        element = (pow(-1, n)*pow(x, 2*n))/fac(2*n);
        sum += element;
        n++;
    } while(fabs(element) > eps);
    return sum;
}

double function_c(double eps, double x){
    if (x >= 1 || x <= -1){
        return 0;
    }
    int n = 0;
    double sum = 1;
    double element= 1;
    do{
        element *= (9*pow(n+1, 2)*pow(x,2))/((3*n+2)*(3*n+1));
        sum += element;
        n++;
    } while(fabs(element) > eps);
    return sum;
}

double function_d(double eps, double x){
    if (x >= 1 || x <= -1){
        return 0;
    }
    int n = 1;
    double sum = -((pow(x, 2))/2);
    double element= -((pow(x, 2))/2);
    do{
        element *= -(((2*n+1)*pow(x, 2))/(2*n+2));
        sum += element;
        n++;
    } while(fabs(element) > eps);
    return sum;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Input must be like: epsilon, x");
        return 0;
    }
    if(number_check(argv[1]) == 0 || number_check(argv[1]) == 2 || eps_check(argv[1]) == 0){
        printf("Wrong epsilon.");
        return 0;
    }
    if(number_check(argv[2]) == 0){
        printf("Wrong x.");
        return 0;
    }
    double eps = atof(argv[1]);
    double x = atof(argv[2]);
    printf("Result of a: %.10f\n", function_a(eps, x));
    printf("Result of b: %.10f\n", function_b(eps, x));
    if (!function_c(eps, x)){
        printf ("Error: wrong x. x must be like: -1 < x < 1\n");
    }
    else {
        printf("Result of c: %.10f\n", function_c(eps, x));
    }
    if (!function_d(eps, x)){
        printf ("Error: wrong x. x must be like: -1 < x < 1\n");
    }
    else{
        printf("Result of d: %.10f\n", function_d(eps, x));
    }
}
