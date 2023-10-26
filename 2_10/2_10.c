#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

enum errors{
    success = 1,
    memory_error = -1
};

void polynom(double eps, int a, double **result, int power, ...);
void print(int state);
int fact (int number);

int main(){
    double epsilon = 0.00001;
    double a = 3.0;
    double *result;
    int power = 4;
    polynom(epsilon, a, &result, power, 1.0, 0.0, -3.0, 1.0, -2.0);
    for(int i = 0; i <= power; i++){
        printf("g%d = %lf\n", i, result[i]);
    }
    free(result);
    return success;
}

void polynom(double eps, int a, double **result, int power, ...){
    *result = (double*)malloc((power+1)*sizeof(double));
    if(*result == NULL){
        print(memory_error);
        return;
    }
    double *coeffs = (double*)malloc((power)*sizeof(double));
    if(coeffs == NULL){
        print(memory_error);
        return;
    }
    va_list coefs;
    va_start (coefs, power);
    double coef;
    for (int i = 0; i < power; i++){
        coef = va_arg(coefs, double);
        coeffs[i] = coef;
    }
    double sum = 0.0;
    for(int i = 0; i < power; i++){
        sum += (coeffs[i]*pow(a, power - i));    
    }
    sum += va_arg(coefs, double);
    (*result)[0] = sum;
    va_end(coefs);
    double *pr = (double*)malloc((power)*sizeof(double));
    if(pr == NULL){
        print(memory_error);
        return;
    }
    int count = 0;
    while (count != power){
        double sum = 0.0;
        for(int i = 0; i <= power - count - 1; i++){
            pr[i] = coeffs[i]*(power-count-i);
            sum += pr[i]*pow(a, power - count - i - 1);
        }
        (*result)[count+1] = sum/fact(count+1);
        for (int i = 0; i <= power - count - 1; i++){
            coeffs[i] = pr[i];
        }
        count++;
    }
    free(coeffs);
    free(pr);
}
int fact (int number){
    if(number == 0 || number == 1){
        return 1;
    }
    else{
        return number*fact(number-1);
    }
}
void print(int state){
    if (state == memory_error){
        printf("Memory error");
    }
}

//2.0, -3.0, 5.0, 1.0 a = -1