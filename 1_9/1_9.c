#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_amount = -1,
    wrong_input = -2
};

void fill_random (int *numbers, int a, int b, int length){
    for (int i = 0; i < length; ++i){
        numbers[i] = rand() % (b - a + 1) + a;
    }
}

void swap_max_min(int *numbers, int length){
    int maxelem = -100000;
    int maxind = 0;
    int minelem = 100000;
    int minind = 0;
    for (int i = 0; i < length; i++){
        if (numbers[i] > maxelem){
            maxelem = numbers[i];
            maxind = i;
        }
        if (numbers[i] < minelem){
            minelem = numbers[i];
            minind = i;
        }
    }
    int temp = numbers[maxind];
    numbers[maxind] = numbers[minind];
    numbers[minind] = temp;
}

int get_random_num (int a, int b){
    return rand()%(b - a + 1) + a;
}

int find_elem (int element, int *arr_b, int length){
    int min_dif = 10000;
    int elem = 0;
    for (int i = 0; i < length; i++){
        if (fabs(arr_b[i] - element) < min_dif){
            min_dif = fabs(arr_b[i] - element);
            elem = arr_b[i];
        }
        if (min_dif == 0){
            break;
        }
    }
    return elem;
}

void fill_arr_c (int *arr_a, int *arr_b, int *arr_c, int length_a, int length_b){
    for (int i = 0; i < length_a; i++){
        arr_c[i] = arr_a[i] + find_elem(arr_a[i], arr_b, length_b);
    }
}

int check_digit (char *number){
    int length = strlen(number);
    for (int i = 0; i < length; ++i){
        if(!(number[i] >= 0 && number[i] <= '9') && number[i] !='-'){
            return wrong_input;
        }
    }
    return success;
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    if (argc != 4){
        printf("Wrong amount");
        return wrong_amount;
    }

    if (check_digit(argv[1]) != success || check_digit(argv[2]) != success || check_digit(argv[3]) != success){
        printf ("Input = numbers");
        return wrong_input;
    }

    int length = atoi(argv[1]);
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    int *numbers = malloc(sizeof(int)*length);

    if(a > b){
        int temp = b;
        b = a;
        a = temp;
    }

    fill_random(numbers, a, b, length);
    printf ("Original array: ");
    for (int i = 0; i < length; i++){
        printf("%d ", numbers[i]);
    }
    printf ("\nArray after swapping: ");
    swap_max_min(numbers, length);
    for (int i = 0; i < length; i++){
        printf("%d ", numbers[i]);
    }
    free(numbers);

    int length_a = 5;//get_random_num(10, 10000);
    int *arr_a = malloc(sizeof(int)*length_a);
    int length_b = 3;//get_random_num(10, 10000);
    int *arr_b = malloc(sizeof(int)*length_b);
    int *arr_c = malloc(sizeof(int)*length_a);

    fill_random(arr_a, -1000, 1000, length_a);
    fill_random(arr_b, -1000, 1000, length_b);
    fill_arr_c(arr_a, arr_b, arr_c, length_a, length_b);
    printf ("\nArray A: ");
    for (int i = 0; i < length_a; i++){
        printf("%d ", arr_a[i]);
    }
    printf ("\nArray B: ");
    for (int i = 0; i < length_b; i++){
        printf("%d ", arr_b[i]);
    }
    printf ("\nArray C: ");
    for (int i = 0; i < length_a; i++){
        printf("%d ", arr_c[i]);
    }

    free(arr_a);
    free(arr_b);
    free(arr_c);
}