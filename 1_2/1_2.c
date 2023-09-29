#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>

#define pi 3.14

int number_check(char *l){
    if(sscanf (l, "%d", &l)){
        return 1;
    }
    else{return 0;}
}

double fac(int n) {
	if (n == 0) { return 1; }
	if (n > 0) { return n * fac(n - 1); }
}


double ex_lim (double eps){
    double n = 1;
    double result_1 = pow(1 + 1/(n), n);
    double result_2 = pow(1+ 1/(n+1), n+1);
    while (fabs (result_2 - result_1) > eps){
        result_1 = result_2;
        n++;
        result_2 = pow(1 + 1/(n+1), n+1);
    }
    return result_1;
}

double ex_row (double eps){
    int n = 0; 
    double sum = 0;
    double element = 0;
    do {
        element = 1/fac(n); 
        sum += element;
        n++;
    } while (fabs(element) > eps);
    return sum;
}

double ex_newton(double eps){
    double a = 2;
    double b = 3;
    double result = (a+b)/2;
    double x = result - (log(result)-1)/(1/result);
    while (fabs(result - x) > eps){
        result = x;
        x = result - (log(result)-1)/(1/result);
    }
    return x;
}

double p_lim (double eps){ 
    double n = 1;
    double result_1 = 0;
    double result_2 = 4;
    while (fabs (result_2 - result_1) > eps){
        result_1 = result_2;
        result_2 *= (4 * n * (n + 1)) / pow(2 * n + 1, 2);;
        n++;
    }
    return result_1;
}

double p_row (double eps){
    int n = 1; 
    double sum = 0;
    double element = 0;
    do {
        element = pow(-1, n-1)/(2*n - 1); 
        sum += element;
        n++;
    } while (fabs(element) > eps);
    return 4*sum;
}

double p_newton(double eps){
    double a = 3;
    double b = 4;
    double result = (a+b)/2;
    double x = result - (cos(result)+1)/(-sin(result));
    while (fabs(result - x) > eps){
        result = x;
        x = result - (cos(result)+1)/(-sin(result));
    }
    return x;
}

double ln_lim (double eps){
    double n = 1;
    double result_1 = n*(pow(2, 1/n) - 1);
    double result_2 = (n+1)*(pow(2, 1/(n+1)) - 1);
    while (fabs (result_2 - result_1) > eps){
        result_1 = result_2;
        n++;
        result_2 = (n+1)*(pow(2, 1/(n+1)) - 1);
    }
    return result_2;
}

double ln_row (double eps){
    int n = 1; 
    double sum = 0;
    double element = 0;
    do {
        element = pow(-1, n-1)/n; 
        sum += element;
        n++;
    } while (fabs(element) > eps);
    return sum;
}

double ln_newton(double eps){
    double a = 0;
    double b = 1;
    double result = (a+b)/2.;
    double x = result - (exp(x)-2)/(exp(x));
    while (fabs(result - x) > eps){
        result = x;
        x = result - (exp(x)-2)/(exp(x));
    }
    return x;
}

double sqrt_lim (double eps){
    double result_1 = 0;
    double result_2 = -0.5;
    while (fabs (result_2 - result_1) > eps){
        result_1 = result_2;
        result_2 = result_1 - pow(result_1, 2)/2 + 1;
    }
    return result_1;
}

double sqrt_row (double eps){
    double total_1 = 1;
    double total_2 = 1;
    double element = 0;
    double power = 0.25;
    do {
        total_1 = total_2;
        element = pow(2, power); 
        total_2 *= element;
        power *= 0.5;
    } while (fabs(total_2 - total_1) > eps);
    return total_1;
}

double sqrt_newton(double eps){
    double a = 1;
    double b = 2;
    double result = (a+b)/2;
    double x = result - (pow(x, 2) - 2)/(2*x);
    while (fabs(result - x) > eps){
        result = x;
        x = result - (pow(x, 2) - 2)/(2*x);
    }
    return x;
}

double C (double m, double k){
    double fact = 1;
    for (int i = k + 1; i <= m; ++i){
        fact *= i;
    }
    return (fact/(fac(m-k)));
}

double gamma_lim (double eps){
    double m = 2;
    double result_1 = 0;
    double result_2 = 0;
    double element = 0;
    do{
        result_1 = result_2;
        double sum = 0;
        double fact = 1;
        for (int i = 1; i <= m; ++i){
            fact *= i;
            element = (C(m,i)*(pow(-1, i)/i)*log(fact));
            sum += element;
        }
        result_2 = sum;
        m++;
    } while(fabs(result_2 - result_1) > eps);
    return result_2;
}

double gamma_row(double eps){
    double epsilon = 0.0000000000001;
    double k = 3;
    double element = 0;
    double sum_1 = 0;
    double sum_2 = 0.5;
    do {
        sum_1 = sum_2;
        element = ((1.0/(pow(floor(sqrt(k)), 2))) - (1.0/k)); 
        sum_2 += element;
        if (element < epsilon){
            sum_1 = 0;
        }
        k++;
    } while (fabs(sum_2 - sum_1) > epsilon);
    return (sum_2 - (pow(pi, 2)/6));
}

double prime (int p) {
    if (p <= 1){
        return 0;
    }
    for (int i = 2; i < p; ++i) {
        if (p % i == 0){
            return 0;
        }
    }
    return 1;
}

double multip (double t) {
    double total = 1;
    for (int i = 2; i <= t; ++i){
        if (prime(i)){
            total *= (i-1.0)/i;
        }
    }
    return total;
}

double gam_lim (double eps){
    int t = 2;
    double result_1 = 0;
    double result_2 = 0;
    double element = 0;
    do{
        result_1 = result_2;
        result_2 = log(t)*multip(t);
        t++;
    } while(fabs(result_2 - result_1) > eps);
    return result_2;
}

double gamma_newton(double eps){
    double a = 0;
    double b = 1;
    double result = (a+b)/2;
    double x = (exp(-result) - gam_lim(result))/exp(-result);
    while (fabs(result - x) > eps){
        result = x;
        x = (exp(-result) - gam_lim(result))/exp(-result);
    }
    return x;
}

int main (int argc, char *argv[]){
    setlocale (LC_ALL, "rus");

    if (argc != 2){
        printf("Необходимо ввести один аргумент: точность вычислений");
    }
    if (!number_check(argv[1])){
        printf ("Введено неправильное число. Число не должно содержать лишних символов в своей записи.\n");
        return 0;
    }
    
    int l = atoi(argv[1]);
    double eps = pow(10, -l);

    printf("e limit: %.*lf\n", 8, ex_lim(eps));
    printf("e row: %.*lf\n", 8, ex_row(eps));
    printf("e function: %.*lf\n", 8, ex_newton(eps));
    printf("____________________\n");
    printf("p limit: %.*lf\n", 8, p_lim(eps));
    printf("p row: %.*lf\n", 8, p_row(eps));
    printf("p function: %.*lf\n", 8, p_newton(eps));
    printf("____________________\n");
    printf("ln2 limit: %.*lf\n", 8, ln_lim(eps));
    printf("ln2 row: %.*lf\n", 8, ln_row(eps));
    printf("ln2 function: %.*lf\n", 8, ln_newton(eps));
    printf("____________________\n");
    printf("sqrt limit: %.*lf\n", 8, sqrt_lim(eps));
    printf("sqrt row: %.*lf\n", 8, sqrt_row(eps));
    printf("sqrt function: %.*lf\n", 8, sqrt_newton(eps));
    printf("____________________\n");
    printf("gamma limit: %.*lf\n", 8, gamma_lim(eps));
    printf("gamma row: %.*lf\n", 8, gamma_row(eps));
    printf("gamma function: %.*lf\n", 8, gamma_newton(eps));
}