#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

enum errors{
    success = 1,
    wrong_input = -2
};

int find_max (int length, char *element, int base){
    int num = 0;
    int temp = 0;
    if(!strcmp(element, "Stop")){
        return num;
    }
    for(int i = 0; i < length; ++i){
        if(isdigit(element[i])){
            temp = element[i] - '0';
            num += (temp*pow(base, length - i - 1));
        }
        else{
            temp = tolower(element[i]) - 'a' + 10;
            num += (temp*pow(base, length - i - 1));
        }
    }
    return num;
}

int to_9_base(int number){
    int answer = 0;
    while (number != 0){
        answer = answer + (number % 9);
        number /= 9;
        answer *= 10;
    }
    int total = 0;
    while (answer != 0){
        total = total*10 + (answer % 10);
        answer /= 10;
    }
    return total;
}

int to_18_36_base(int number, char *answer, int base){
    int i = 0;
    while (number != 0){
        if (number % base >= 10){
            answer[i] = (number % base) % 10 + 'A';
        }
        else{
            answer[i] = (number % base) + '0';
        }
        number /= base;
        i++;
    }
    return i;
}

int main (){
    printf("Input system: ");
    int base = 0;
    if (!(scanf("%d", &base))){
        printf("Input must be a number");
        return wrong_input;
    }
    if (base < 2 || base > 36){
        printf("System must be a number between 2 and 36");
        return wrong_input;
    }
    char element[30] = {0};
    int length = 0;
    int max_element = 0;
    int elem = 0;
    int size = 0;
    printf("Write numbers (Stop = end)\n");
    do{
        scanf("%s", &element);
        length = strlen(element);
        elem = find_max(length, element, base);
        if (elem > max_element){
            max_element = elem;
            size = length;
        }
    } while(strcmp(element, "Stop"));
    printf("\nMax number %d", max_element);

    printf("\nMax number in 9: %d\n", to_9_base(max_element));

    char *answer_18 = malloc(sizeof(char)*size);
    int count = to_18_36_base(max_element, answer_18, 18);
    printf("Max number in 18: ");
    for (int i = count-1; i >= 0; --i){
        printf("%c", answer_18[i]);
    }

    char *answer_27 = malloc(sizeof(char)*size);
    count = to_18_36_base(max_element, answer_27, 27);
    printf("\nMax number in 27: ");
    for (int i = count-1; i >= 0; --i){
        printf("%c", answer_27[i]);
    }

    char *answer_36 = malloc(sizeof(char)*size);
    count = to_18_36_base(max_element, answer_36, 36);
    printf("\nMax number in 36: ");
    for (int i = count-1; i >= 0; --i){
        printf("%c", answer_36[i]);
    }

    free(answer_18);
    free(answer_27);
    free(answer_36);
    return 0;
}