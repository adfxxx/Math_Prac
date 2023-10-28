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
    double A[] = {1.0, 0.0};
    double B[] = {2.0, 0.0};
    double C[] = {1.0, 1.0};
    double D[] = {2.0, 1.0};
    double E[] = {1.33, 5.1};
    // if(check_convex(5, A, B, C, D, E) == success){
    //     printf("Is polygon convex: Yes\n");
        
    // }

    if(check_convex(5, B, D, E, C, A) == success){
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