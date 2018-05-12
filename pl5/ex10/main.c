#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_OF_PRODUCTS 5
#define NUM_OF_HYPERMARKETS 3

#define ARRAY_SIZE 1000 //array_size is 10000
#define FILTERING_THREADS 3
#define COMPUTING_THREADS 3
#define MAX_THREADS FILTERING_THREADS + COMPUTING_THREADS
#define THREAD_STEP ARRAY_SIZE / FILTERING_THREADS

#define MIN_PRICE 100
#define MAX_PRICE 350
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

const unsigned int V_HYPER_ID[] = {1, 2, 3};
const unsigned int V_PROD_ID[] = {32, 64, 128, 256, 512};

/*
    Lowest cost of 5 products average
*/

LowestCostHypermarket lowestcost;

/*
    Products filtered by hypermarket
*/

ProductsFromHypermarket vec[NUM_OF_HYPERMARKETS] = {
    {.id_h = 1, .n_elements = 0}, //1st hm
    {.id_h = 2, .n_elements = 0}, //2nd hm
    {.id_h = 3, .n_elements = 0}  //3nd hm
};

/*
    Database
*/

ProductsFromHypermarket database = {
    .id_h = 0, .n_elements = 0};

/*
    Mutex
*/
pthread_mutex_t filtering[FILTERING_THREADS];
pthread_mutex_t mutex_lowestcost = PTHREAD_MUTEX_INITIALIZER;

/*
    Threads
*/
pthread_t THREADS[MAX_THREADS];
/*
    Functions prototypes
*/
void populate_database();
void showLowestCostHypermarket();
void *filter_function_thread(void *args);
int main()
{
    populate_database();
    int i;
    int indexes[MAX_THREADS];
    for (i = 0; i < MAX_THREADS; i++)
    {
        indexes[i] = i;
        //Criar filtering threads
        if (i < FILTERING_THREADS)
        {
            //
            // Criar computing threads
        }
        else
        {
        }
    }
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
        p->id_p = prod_id;
        p->p = (rand() % (MAX_PRICE + 1 - MIN_PRICE)) + MIN_PRICE;
    }
}

void showLowestCostHypermarket()
{
    printf("The HM:%d is the lowest cost with an average of :%d", lowestcost.id_h, lowestcost.price);
}

/*================FILTERING THREADS===============*/

void *filter_function_thread(void *args)
{
    int t_id = *(int *)args;
    int initial=t_id*THREAD_STEP;
    int limit=initial+THREAD_STEP;
    int i;
    for(i=initial;i<limit;++i){
        Product *p = database.Vec[i];
    }

    pthread_exit(0);
}