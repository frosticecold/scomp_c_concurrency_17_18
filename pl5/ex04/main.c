#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define COLUMNS 5
#define LINES 5
#define MAX_NUMBER 100
#define MAX_THREADS 5
typedef struct
{

    int **matrix1;
    int **matrix2;
    int **result;
} matrices;

typedef struct
{
    int index;
    matrices *tm;

} shared_data;

int **creatematrix(int lines, int columns);
void *fill_matrix_thread(void *args);
void print_matrix(int **matrix, int lines, int columns);
void *multiply_matrices(void *args);
void empty_matrix(matrices *tm);
int main()
{

    pthread_t fill1, fill2;

    matrices tm;
    tm.matrix1 = creatematrix(LINES, COLUMNS);
    tm.matrix2 = creatematrix(LINES, COLUMNS);
    tm.result = creatematrix(LINES, COLUMNS);
    pthread_create(&fill1, NULL, fill_matrix_thread, &tm.matrix1);
    pthread_create(&fill2, NULL, fill_matrix_thread, &tm.matrix2);

    empty_matrix(&tm);

    pthread_join(fill1, NULL);
    pthread_join(fill2, NULL);

    print_matrix(tm.matrix1, LINES, COLUMNS);
    print_matrix(tm.matrix2, LINES, COLUMNS);
    pthread_t threads[MAX_THREADS];
    shared_data sd_threads[MAX_THREADS];
    int i;
    for (i = 0; i < MAX_THREADS; ++i)
    {
        sd_threads[i].index = i;
        sd_threads[i].tm = &tm;
        pthread_create(&threads[i], NULL, multiply_matrices, &sd_threads[i]);
    }

    for (i = 0; i < MAX_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    print_matrix(tm.result, LINES, COLUMNS);
    return 0;
}

/*
    Creates a 2D Matrix in the heap
*/
int **creatematrix(int lines, int columns)
{
    int **matrix = (int **)malloc(lines * sizeof(int *));
    int i;
    for (i = 0; i < columns; i++)
        matrix[i] = (int *)malloc(columns * sizeof(int));

    return matrix;
}
/*
    Function for a thread to fill a matrix with random numbers
*/
void *fill_matrix_thread(void *args)
{
    int ***endereco = args;
    int **matrix = *endereco;
    srand(time(NULL) * pthread_self());
    int i; // lines
    int j; // columns
    for (i = 0; i < LINES; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }
    pthread_exit(0);
}

void empty_matrix(matrices *tm)
{
    int i; // lines
    int j; // columns
    for (i = 0; i < LINES; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            tm->result[i][j] = 0;
        }
    }
}
void print_matrix(int **matrix, int lines, int columns)
{
    int i, j;
    printf("=================\n");
    for (i = 0; i < lines; i++)
    {
        for (j = 0; j < columns; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("=================\n");
}

void *multiply_matrices(void *args)
{
    shared_data *sd = (shared_data *)args;
    matrices *tm = sd->tm;
    const int NUM_OF_MULTS = 5;
    int index = sd->index;

    int i, j, k;
    for (i = index; i < index + 1; ++i)
    {
        for (j = 0; j < NUM_OF_MULTS; ++j)
        {
            for (k = 0; k < NUM_OF_MULTS; ++k)
            {
                tm->result[i][j] += tm->matrix1[i][k] * tm->matrix2[k][j];
            }
        }
    }

   pthread_exit(0);
}