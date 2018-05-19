#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#define NUM_STUDENTS 5
#define MAX_GRADE 101
#define MINIMUN_FOR_POSITIVE 50
#define NUMBER_OF_EVALUATIONS 3
#define NOT_GRADED 0
#define GRADE_INSERTED 1
#define GRADE_EVALUATED 2

typedef struct
{
    int number;
    int notaG1;
    int notaG2;
    int notaG3;
    int notaFinal;
} Prova;

pthread_t thread_G1, thread_G2, thread_G3;
pthread_t thread_positive_grade, thread_negative_grade;

pthread_cond_t thread_cond_evaluation = PTHREAD_COND_INITIALIZER;

pthread_cond_t thread_cond_positive_grade = PTHREAD_COND_INITIALIZER, thread_cond_negative_grade = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_condition_grades = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_condition_result_grade = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_cond_wait_for_grade = PTHREAD_COND_INITIALIZER;

int positive_grade = 0;
int negative_grade = 0;

int total_graded = 0;
int next_graded = 0;
int evaluated = 0;

int total_students = 0;

typedef struct
{
    Prova provas[NUM_STUDENTS];
    int check[NUM_STUDENTS];
} Prova_array;

Prova_array provas_array;
/*----------*/

void *func_thread_grade_students(void *);
void *func_thread_calc_average(void *);

void *func_thread_positive_grade(void *);
void *func_thread_negative_grade(void *);

void initialize_threads();

int get_average(int);
/*----------*/

int main()
{
    srand(time(NULL));
    initialize_threads();
    int i = 0;
    for (; i < NUM_STUDENTS; i++)
    {
        printf("Final grade %d is %d\n", i, provas_array.provas[i].notaFinal);
    }
    printf("positive grades : %d\n", positive_grade);
    printf("negative grades : %d\n", negative_grade);
    return 0;
}

void initialize_threads()
{

    if (pthread_create(&thread_G1, NULL, func_thread_grade_students, (void *)NULL) != 0)
    { //confirming error for creating thread
        perror("error creating thread G1\n");
    }
    if (pthread_create(&thread_G2, NULL, func_thread_calc_average, (void *)NULL) != 0)
    { //confirming error for creating thread
        perror("error creating thread G2\n");
    }

    if (pthread_create(&thread_G3, NULL, func_thread_calc_average, (void *)NULL) != 0)
    { //confirming error for creating thread
        perror("error creating thread G3\n");
    }

    if (pthread_create(&thread_positive_grade, NULL, func_thread_positive_grade, (void *)NULL) != 0)
    { //confirming error for creating thread
        perror("error creating thread positive grade\n");
    }
    if (pthread_create(&thread_negative_grade, NULL, func_thread_negative_grade, (void *)NULL) != 0)
    { //confirming error for creating thread
        perror("error creating thread negative grade\n");
    }

    pthread_join(thread_G1, NULL);
    pthread_join(thread_G2, NULL);
    pthread_join(thread_G3, NULL);

    pthread_join(thread_positive_grade, NULL);
    pthread_join(thread_negative_grade, NULL);
}

void *func_thread_grade_students(void *args)
{

    int i = 0;
    for (; i < NUM_STUDENTS; i++)
    {
        int grade = rand() % (MAX_GRADE);
        provas_array.provas[i].notaG1 = grade;
        grade = rand() % (MAX_GRADE);
        provas_array.provas[i].notaG2 = grade;
        grade = rand() % (MAX_GRADE);
        provas_array.provas[i].notaG3 = grade;

        provas_array.check[i] = GRADE_INSERTED;
        pthread_mutex_lock(&mutex_condition_grades);
        total_graded++;
        pthread_cond_signal(&thread_cond_evaluation);
        pthread_mutex_unlock(&mutex_condition_grades);
    }
    pthread_exit(NULL);
}

void *func_thread_calc_average(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&mutex_condition_grades);
        if (next_graded == NUM_STUDENTS)
        {
            pthread_mutex_unlock(&mutex_condition_grades);
            pthread_exit(NULL);
        }
        while (next_graded == total_graded)
        {

            pthread_cond_wait(&thread_cond_evaluation, &mutex_condition_grades);
            if (next_graded == NUM_STUDENTS) // check if its already over
            {
                pthread_mutex_unlock(&mutex_condition_grades);
                pthread_exit(NULL);
            }
        }

        if (provas_array.check[next_graded] == GRADE_INSERTED)
        {
            provas_array.provas[next_graded].notaFinal = get_average(next_graded);
            provas_array.check[next_graded] = GRADE_EVALUATED;
            int temp = provas_array.provas[next_graded].notaFinal;
            pthread_mutex_lock(&mutex_condition_result_grade);

            next_graded++;
            if (temp >= MINIMUN_FOR_POSITIVE)
            {
                pthread_cond_signal(&thread_cond_positive_grade);
                printf("positive %d \n", positive_grade);
            }
            else
            {
                pthread_cond_signal(&thread_cond_negative_grade);
                printf("negtive %d\n", negative_grade);
            }

            pthread_mutex_unlock(&mutex_condition_result_grade);
        }

        pthread_mutex_unlock(&mutex_condition_grades);
    }
    pthread_exit(NULL);
}

void *func_thread_positive_grade(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&mutex_condition_result_grade);

        if ((negative_grade + positive_grade) == NUM_STUDENTS)
        {
            pthread_mutex_unlock(&mutex_condition_result_grade);
            break;
        }
        printf("dentro da tread\n");
        while (next_graded == (negative_grade + positive_grade))
        {
            printf("while next %d\n", next_graded);

            printf("while sum %d\n", negative_grade + positive_grade);
            pthread_cond_wait(&thread_cond_positive_grade, &mutex_condition_result_grade);
            if ((negative_grade + positive_grade) == NUM_STUDENTS) // check if its already over
            {
                pthread_mutex_unlock(&mutex_condition_result_grade);
                pthread_exit(NULL);
            }
        }

        positive_grade++;

        pthread_mutex_unlock(&mutex_condition_result_grade);
        pthread_cond_signal(&thread_cond_wait_for_grade);
    }
    pthread_exit(NULL);
}

void *func_thread_negative_grade(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&mutex_condition_result_grade);
        printf("dentro da tread\n");
        if ((negative_grade + positive_grade) == NUM_STUDENTS)
        {
            printf("while next %d\n", next_graded);

            printf("while sum %d\n", negative_grade + positive_grade);
            pthread_mutex_unlock(&mutex_condition_result_grade);
            pthread_exit(NULL);
        }

        while (next_graded == (negative_grade + positive_grade))
        {

            pthread_cond_wait(&thread_cond_negative_grade, &mutex_condition_result_grade);
            if ((negative_grade + positive_grade) == NUM_STUDENTS) // check if its already over
            {
                pthread_mutex_unlock(&mutex_condition_result_grade);
                pthread_exit(NULL);
            }
        }

        negative_grade++;

        pthread_mutex_unlock(&mutex_condition_result_grade);
        pthread_cond_signal(&thread_cond_wait_for_grade);
    }

    pthread_exit(NULL);
}

int get_average(int i)
{
    return (provas_array.provas[i].notaG1 + provas_array.provas[i].notaG2 + provas_array.provas[i].notaG3) / NUMBER_OF_EVALUATIONS;
}
