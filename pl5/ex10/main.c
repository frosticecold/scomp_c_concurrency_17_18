#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define NUM_OF_PRODUCTS 5
#define NUM_OF_HYPERMARKETS 3

#define ARRAY_SIZE 1000 //array_size is 10000
#define FILTERING_THREADS 3
#define COMPUTING_THREADS 3
#define MAX_THREADS FILTERING_THREADS + COMPUTING_THREADS
#define THREAD_STEP ARRAY_SIZE / FILTERING_THREADS

#define MIN_PRICE 100
#define MAX_PRICE 350

#define WALL_VARIABLE_MAX 3
typedef struct
{
    unsigned int id_h;
    unsigned int id_p;
    unsigned int p;
} Product;

typedef struct
{
    unsigned int id_h;
    Product Vec[ARRAY_SIZE];
    int n_elements;
} ProductsFromHypermarket;

typedef struct
{
    int price;
    int id_h;

} LowestCostHypermarket;

const unsigned int V_HYPER_ID[] = {0, 1, 2};
const unsigned int V_PROD_ID[] = {32, 64, 128, 256, 512};

/*
    Lowest cost of 5 products average
*/

LowestCostHypermarket lowestcost = {
    .price = INT_MAX};

/*
    Products filtered by hypermarket
*/

ProductsFromHypermarket vector_of_products[NUM_OF_HYPERMARKETS] = {
    {.id_h = 0, .n_elements = 0}, //1st hm
    {.id_h = 1, .n_elements = 0}, //2nd hm
    {.id_h = 2, .n_elements = 0}  //3nd hm
};

/*
    Database
*/

ProductsFromHypermarket database = {
    .id_h = 0, .n_elements = 0};

/*
    Mutex
*/
pthread_mutex_t mutex_filtering[FILTERING_THREADS];
pthread_mutex_t mutex_lowestcost = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_wall = PTHREAD_MUTEX_INITIALIZER;
/*
    Cond
*/
//pthread_cond_t computing_condition[FILTERING_THREADS];
pthread_cond_t computing_condition = PTHREAD_COND_INITIALIZER;
/*
    threads
*/
pthread_t threads[MAX_THREADS];
/*
    Functions prototypes
*/

/*
    Wall variable
*/
unsigned int wall_variable = 0;

void populate_database();
void create_sync_data();
void showLowestCostHypermarket();
void *filter_function_thread(void *args);
void *computing_function_thread(void *args);
int main()
{
    populate_database();
    create_sync_data();
    int i;
    int indexes[MAX_THREADS];
    /*
        Create threads
    */
    for (i = 0; i < MAX_THREADS; i++)
    {

        //Criar filtering threads
        if (i < FILTERING_THREADS)
        {
            indexes[i] = i;
            // Criar computing threads
            if (pthread_create(&threads[i], NULL, filter_function_thread, &indexes[i]) != 0)
            {
                perror("Erro ao criar filter thread.");
                exit(1);
            }
        }
        else
        {
            indexes[i] = i - FILTERING_THREADS;
            if (pthread_create(&threads[i], NULL, computing_function_thread, &indexes[i]) != 0)
            {
                perror("Erro ao criar computing thread");
                exit(1);
            }
        }
    }
    /*
    Wait for filtering threads
    */
    printf("Waiting for filtering threads..\n");
    for (i = 0; i < FILTERING_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Erro pthread join");
            exit(1);
        }
    }
    /*
        Wait for computing threads
    */
    printf("Waiting for computing threads..\n");
    for (i = FILTERING_THREADS; i < MAX_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Erro pthread join");
            exit(1);
        }
    }
    showLowestCostHypermarket();
    return 0;
}

void populate_database()
{
    database.id_h = 0;
    srand(time(NULL));
    //const int N_PRODUCTS_TO_GENERATE = (ARRAY_SIZE / NUM_OF_HYPERMARKETS);
    int i, hm_id, prod_id;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        hm_id = rand() % NUM_OF_HYPERMARKETS; // generate between 0-2 corresponding to hm 1-3
        prod_id = rand() % NUM_OF_PRODUCTS;   // generate between 0-4 corresponding to p_id 1-5
        database.n_elements++;
        Product *p = &database.Vec[i];
        p->id_h = V_HYPER_ID[hm_id];
        p->id_p = V_PROD_ID[prod_id];
        p->p = (rand() % (MAX_PRICE + 1 - MIN_PRICE)) + MIN_PRICE;
    }
}

void create_sync_data()
{
    int i;
    for (i = 0; i < FILTERING_THREADS; ++i)
    {
        if (pthread_mutex_init(&mutex_filtering[i], NULL) != 0)
        {
            perror("Error initiating mutex.");
            exit(1);
        }
        /*
        if (pthread_cond_init(&computing_condition[i], NULL) != 0)
        {
            perror("Erro criar computing_condition");
            exit(1);
        }
        */
    }
    if (pthread_mutex_init(&mutex_lowestcost, NULL) != 0)
    {
        perror("Error mutex_lowestcost");
        exit(1);
    }
}
void showLowestCostHypermarket()
{
    printf("The HM:%d is the lowest cost with an average of :%d", lowestcost.id_h, lowestcost.price);
}

/*================FILTERING threads===============*/

void *filter_function_thread(void *args)
{
    int t_id = *(int *)args;
    printf("Filter Thread id :%d\n", t_id);
    int initial = t_id * THREAD_STEP;
    int limit = initial + THREAD_STEP;
    int i;
    for (i = initial; i < limit; ++i)
    {
        Product *p = &database.Vec[i];
        int hm_id = p->id_h;
        ProductsFromHypermarket *vec = &vector_of_products[hm_id];

        pthread_mutex_lock(&mutex_filtering[hm_id]);
        if (vec->n_elements < ARRAY_SIZE)
        {
            memcpy(&vec->Vec[vec->n_elements], p, sizeof(Product));
            vec->n_elements++;
        }
        pthread_mutex_unlock(&mutex_filtering[hm_id]);
    }
    pthread_mutex_lock(&mutex_wall);
    wall_variable++;
    if (wall_variable >= WALL_VARIABLE_MAX)
    {
        pthread_cond_broadcast(&computing_condition);
    }
    pthread_mutex_unlock(&mutex_wall);
    //pthread_cond_signal(&computing_condition[t_id]);
    pthread_exit(0);
}

void *computing_function_thread(void *args)
{
    int t_id = *(int *)args;
    int i, p_id;
    printf("Computing Thread id :%d\n", t_id + COMPUTING_THREADS);
    pthread_mutex_lock(&mutex_wall);
    while (wall_variable < WALL_VARIABLE_MAX)
        pthread_cond_wait(&computing_condition, &mutex_wall);
    pthread_mutex_unlock(&mutex_wall);
    Product products[NUM_OF_PRODUCTS];
    int n_elements[NUM_OF_PRODUCTS];
    /*
        Initialize to zero
    */
    for (i = 0; i < NUM_OF_PRODUCTS; i++)
    {
        products[i].id_h = t_id;
        products[i].id_p = V_PROD_ID[i];
        products[i].p = 0;
        n_elements[i] = 0;
    }
    for (i = 0; i < vector_of_products[t_id].n_elements; ++i)
    {
        Product *p = &vector_of_products[t_id].Vec[i];
        for (p_id = 0; p_id < NUM_OF_PRODUCTS; p_id++)
        {
            if (p->id_p == products[p_id].id_p)
            {
                products[p_id].p += p->p;
                n_elements[p_id]++;
            }
        }
    }
    int average = 0;
    for (i = 0; i < NUM_OF_PRODUCTS; i++)
    {
        if (n_elements[i] > 0)
        {
            average += (int)(products[i].p / n_elements[i]);
        }
    }
    printf("Computing Thread %d HM:%d price:%d\n", t_id + COMPUTING_THREADS, t_id, average);
    pthread_mutex_lock(&mutex_lowestcost);
    if (lowestcost.price > average)
    {
        lowestcost.id_h = t_id;
        lowestcost.price = average;
    }
    pthread_mutex_unlock(&mutex_lowestcost);

    pthread_exit(0);
}