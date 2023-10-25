#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

enum errors{
    success = 1,
    fail = -1,
    wrong_system = -2,
    wrong_input = -3
};

void fraction(int system, ...);
void print(int state, double fraction);
int is_period(double fraction, int system);
int get_denumer(int numer);

int main(){
    int system = 4;
    fraction(system, 0.5, 0.007, 0.6, 0.4353, -1.0);
}

void fraction(int system, ...){
    double eps = 0.0000001;
    if(system < 2 || system > 36){
        print(wrong_system, 0);
        return;
    }
    va_list fractions;
    va_start(fractions, system);
    double fraction;
    while ((fraction = va_arg(fractions, double)) != -1.0){
        if (fraction < eps || (fraction - 1) > eps){
            va_end(fractions);
            print(wrong_input, 0);
            return;
        }
        if(is_period(fraction, system) == success){
            print(success, fraction);
        }
        else{
            print(fail, fraction);
        }
        fraction = va_arg(fractions, double);
    }
    va_end(fractions);
}

int is_period(double fraction, int system){
    double eps = 0.0000001;
    double main_part;
    double fract_part = modf(fraction, &main_part);
    while(fract_part > eps){
        fraction *= 10;
        fract_part = modf(fraction, &main_part);
    }

    int main_fract = (int)main_part;
    int denumer = get_denumer(main_fract);
    while (denumer % 2 == 0){
        if(system % 2 != 0){
            return fail;
        }
        denumer /= 2;
    }
    for (int i = 3; i < sqrt(denumer); i+=2){
        while(denumer % i == 0){
            if(system % i != 0){
                return fail;
            }
            denumer /= i;
        }
    }
    if(denumer > 1){
        if(system % denumer != 0){
            return fail;
        }
    }
    else{
        return success;;
    }
}

int get_denumer(int numer){
    char buff[256];
    sprintf(buff, "%d", numer);
    int length = strlen(buff);
    int denumer = 1;
    for (int i = 0; i < length; i++){
        denumer *= 10;
    }
    while (numer % 2 == 0 && denumer % 2 == 0){
        numer /= 2;
        denumer /= 2;
    }
    for (int i = 3; i < sqrt(numer); i += 2){
        while(numer % i == 0 && denumer % i == 0){
            numer /= i;
            denumer /= i;
        }
    }
    if (numer > 2){
        if (denumer % numer == 0){
            denumer /= numer;
        }
    }
    return denumer;
}

void print(int state, double fraction){
    if(state == success){
        printf("Fraction %lf is not periodic\n", fraction);
    }
    else if(state == fail){
        printf("Fraction %lf is periodic\n", fraction);
    }
    else if(state == wrong_system){
        printf("System is wrong\n");
    }
    else if(state == wrong_input){
        printf("Input is wrong\n");
    }
}