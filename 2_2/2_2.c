#include <stdio.h>
#include <stdarg.h>
#include <math.h>

enum errors{
    success = 1,
    wrong_input = -1,
    calculation_error = -2
};

double get_geom(int size, ...);
double quick_pow(double number, int power);

int main(){
    double result = get_geom(2, 1.0, -1.0);
    if(result == wrong_input){
        printf("Wrong_input\n");
    }
    else if (isnan(result)){
        printf("Error in calculations\n");
    } 
    else{
        printf("Geometric mean: %lf\n", result);
    }

    result = quick_pow(5.0, -2);
    printf("Number in power: %lf", result);
    return success;
}

double get_geom(int size, ...){
    if(size <= 0){
        return wrong_input;
    }
    double result = 1.0;
    double num = 0;
    va_list numbers;
    va_start (numbers, size);
    for(int i = 0; i < size; i++){
        num = va_arg(numbers, double);
        if(num == 0){
            va_end(numbers);
            return wrong_input;
        }
        result *= num;
    }
    va_end(numbers);
    return pow(result, 1.0/size);
}

double quick_pow(double number, int power){
    if(number == 0.0){
        return number;
    }
    if(power == 0){
        return 1.0;
    }
    double num;
    if(power % 2 == 0){
        num = quick_pow(number, power/2);
        return num*num;
    }
    else if(power < 0){
        num = quick_pow(number, (-power/2));
        return 1.0/(num*num*number);
    }
    else{
        num = quick_pow(number, power/2);
        return num*num*number;
    }
}