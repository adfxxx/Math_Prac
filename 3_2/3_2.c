#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

enum errors{
    success = 1,
    memory_error = -1
};

typedef struct{
    double *coords;
    int size;
}Vector;

int find_norm(int count, Vector **max_norm, int *count_1, int *count_p, int *count_m, Vector **max_p_norm, Vector **max_A_norm, int p, int size, double **A, double(*get_norm)(Vector), 
double (*get_p_norm)(Vector, double), double(*get_matrix_norm)(Vector, double **, int), ...);
double get_norm(Vector v);
double get_p_norm(Vector v, double p);
double get_matrix_norm(Vector v, double **A, int size);
void print(int state);

int main(){
    int size = 3;
    int vector_count = 3;
    double **A = (double**)malloc(size*sizeof(double*));
    if(A == NULL){
        print(memory_error);
        return memory_error;
    }
    for(int i = 0; i < size; i++){
        A[i] = (double *)malloc(size*sizeof(double));
        if(A[i] == NULL){
            print(memory_error);
            for(int j = 0; j < i; j++){
                free(A[j]);
            }
            free(A);
            return memory_error;
        }
    }
    A[0][0] = 2; A[1][0] = 0; A[2][0] = 0;
    A[0][1] = 0; A[1][1] = 3; A[2][1] = 0;
    A[0][2] = 0; A[1][2] = 0; A[2][2] = 4;


    Vector v1;
    v1.coords = (double[]){1.0, 2.0, 3.0}; //sqrt(14)
    v1.size = size;
    Vector v2;
    v2.coords = (double[]){-1.0, 2.0, -3.0};
    //v2.coords = (double[]){5.0, 7.0, -10.0}; //sqrt(174)
    v2.size = size;
    Vector v3;
    v3.coords = (double[]){0.0, 0.0, 0.0};
    v3.size = size;

    Vector *max_norm = (Vector*)malloc(10*sizeof(Vector));
    Vector *max_p_norm = (Vector*)malloc(10*sizeof(Vector));
    Vector *max_A_norm = (Vector*)malloc(10*sizeof(Vector));
    int count_1 = 0;
    int count_p = 0;
    int count_m = 0;

    int p = 2;
    
    int result = find_norm(3, &max_norm, &count_1, &count_p, &count_m, &max_p_norm, &max_A_norm, p, size, A, get_norm, get_p_norm, get_matrix_norm, v1, v2, v3);
    if(result == memory_error){
        for(int i = 0; i < size; i++){
            free(A[i]);
        }
        free(A);
        free(max_norm);
        free(max_p_norm);
        free(max_A_norm);
        return memory_error;
    }

    printf("Max vectors:\n");
    for(int i = 0; i < count_1; i++){
        printf("\tvector %d: [", i+1);
        for(int j = 0; j < max_norm[i].size; j++){
            printf("%lf", max_norm[i].coords[j]);
            if(j < max_norm[i].size - 1){
                printf(" ");
            }
        }
        printf("]\n");
    }
    printf("Max p-norm vectors:\n");
    for(int i = 0; i < count_p; i++){
        printf("\tvector %d: [", i+1);
        for(int j = 0; j < max_p_norm[i].size; j++){
            printf("%lf", max_p_norm[i].coords[j]);
            if(j < max_p_norm[i].size - 1){
                printf(" ");
            }
        }
        printf("]\n");
    }
    printf("Max matrix norm vector:\n");
    for(int i = 0; i < count_m; i++){
        printf("\tvector %d: [", i+1);
        for(int j = 0; j < max_A_norm[i].size; j++){
            printf("%lf", max_A_norm[i].coords[j]);
            if(j < max_A_norm[i].size - 1){
                printf(" ");
            }
        }
        printf("]\n");
    }

    for(int i = 0; i < size; i++){
        free(A[i]);
    }
    free(A);
    free(max_norm);
    free(max_p_norm);
    free(max_A_norm);
}

int find_norm(int count, Vector **max_norm, int *count_1, int *count_p, int *count_m, Vector **max_p_norm, Vector **max_A_norm, int p, int size, double **A, double(*get_norm)(Vector), 
double (*get_p_norm)(Vector, double), double(*get_matrix_norm)(Vector, double **, int), ...){
    va_list vectors;
    va_start(vectors, get_matrix_norm);
    double cur_norm;
    double cur_p_norm;
    double cur_matrix_norm;
    double cur_max_norm = -1.0;
    double cur_max_p_norm = -1.0;
    double cur_max_matrix_norm = -1.0;
    int i = 10;
    int j = 10;
    int k = 10;
    Vector *temp = NULL;

    for(int i = 0; i < count; i++){
        Vector vector = va_arg(vectors, Vector);
        cur_norm = get_norm(vector);
        if(cur_norm >= cur_max_norm){
            if(cur_norm > cur_max_norm){
                *count_1 = 0;
                free(*max_norm);
                *max_norm = (Vector*)malloc(10*sizeof(Vector));
                cur_max_norm = cur_norm;
                (*max_norm)[*count_1] = vector;
                (*count_1)++;
            }
            else{
                if(*count_1 == i){
                    temp = (Vector *)realloc(*max_norm, 2*(*count_1)*sizeof(Vector));
                    if(temp == NULL){
                        return memory_error;
                    }
                    *max_norm = temp;
                    i *= 2;
                } 
                (*max_norm)[*count_1] = vector;
                (*count_1)++;
            }
        }
        cur_p_norm = get_p_norm(vector, p);
        if(cur_p_norm >= cur_max_p_norm){
            if(cur_p_norm > cur_max_p_norm){
                *count_p = 0;
                free(*max_p_norm);
                *max_p_norm = (Vector*)malloc(10*sizeof(Vector));
                cur_max_p_norm = cur_p_norm;
                (*max_p_norm)[*count_p] = vector;
                (*count_p)++;
            }
            else{
                if(*count_p == i){
                    temp = (Vector *)realloc(*max_p_norm, 2*(*count_p)*sizeof(Vector));
                    if(temp == NULL){
                        return memory_error;
                    }
                    *max_p_norm = temp;
                    i *= 2;
                } 
                (*max_p_norm)[*count_p] = vector;
                (*count_p)++;
            }
        }
        cur_matrix_norm = get_matrix_norm(vector, A, size);
        if(cur_matrix_norm >= cur_max_matrix_norm){
            if(cur_matrix_norm > cur_max_matrix_norm){
                *count_m = 0;
                free(*max_A_norm);
                *max_A_norm = (Vector*)malloc(10*sizeof(Vector));
                cur_max_matrix_norm = cur_matrix_norm;
                (*max_A_norm)[*count_m] = vector;
                (*count_m)++;
            }
            else{
                if(*count_m == i){
                    temp = (Vector *)realloc(*max_A_norm, 2*(*count_m)*sizeof(Vector));
                    if(temp == NULL){
                        return memory_error;
                    }
                    *max_A_norm = temp;
                    i *= 2;
                } 
                (*max_A_norm)[*count_m] = vector;
                (*count_m)++;
            }
        }
    }
    va_end(vectors);
    return success;
}

double get_norm(Vector v){
    double norm = 0.0;
    for(int i = 0; i < v.size; i++){
        norm += v.coords[i]*v.coords[i];
    }
    return sqrt(norm);
}

double get_p_norm(Vector v, double p){
    double norm = 0.0;
    for(int i = 0; i < v.size; i++){
        norm += pow(fabs(v.coords[i]), p);
    }
    return pow(norm, 1.0/p);
}

double get_matrix_norm(Vector v, double **A, int size){
    double norm = 0.0;
    for(int i = 0; i < v.size; i++){
        for(int j = 0; j < v.size; j++){
            norm += v.coords[i]*A[i][j]*v.coords[j];
        }
    }
    return sqrt(norm);
}

void print(int state){
    if(state == memory_error){
        printf("Memory error\n");
    }
}