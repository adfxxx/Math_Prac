#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

enum errors{
    success = 1,
    fail = -1
};

int check_convex(int sides, ...);
double polynom(double point, int power, ...);

int main(){
    double A[] = {-4.34, 4.65};
    double B[] = {-0.26, 4.57};
    double C[] = {-0.44, -0.63};
    double D[] = {4.16, -0.65};
    double E[] = {-0.03, 5.3};
    if(check_convex(5, A, B, C, D, E) == success){
        printf("Is polygon convex: Yes\n");
        
    }
    else{
        printf("Is polygon convex: No\n");
    }

    double point = -0.678;
    int power = 3;
    printf("Result of polynom: %lf", polynom(point, power, 0.0001, -2.84567, 17.9, 4.8));
    return success;
}

double polynom(double point, int power, ...){
    va_list coefs;
    va_start(coefs, power);
    double result = 0;
    for (int i = 0; i <= power; i++){
        result = result*point + va_arg(coefs, double);
    }
    va_end(coefs);
    return result;
}

int check_convex(int sides, ...){
    va_list coords;
    va_start(coords, sides);
    double *first_dot = va_arg(coords, double*);
    double *second_dot = va_arg(coords, double*);
    int j = 0;
    int k = 0;
    int flag = 0;
    for (int i = 2; i < sides; i++){
        double *now_dot = va_arg(coords, double*);
        double vector_x_1 = second_dot[0] - first_dot[0];
        double vector_y_1 = second_dot[1] - first_dot[1];

        double vector_x_2 = now_dot[0] - first_dot[0];
        double vector_y_2 = now_dot[1] - first_dot[1];

        double vector_mult = vector_x_1 * vector_y_2 - vector_y_1*vector_x_2;
        if (vector_mult > 0){
            flag++;
        }
        else{
            flag--;
        }
        second_dot = now_dot;
    }
    va_end(coords);
    if (flag == sides - 2 || flag == 2 - sides){
        return success;
    }
    else{return fail;}
}