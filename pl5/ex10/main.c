#include <pthread.h>

#define NUM_OF_PRODUCTS 5
#define NUM_OF_HYPERMARKETS 3
#define NUM_OF_CHARS 3
#define MAX_THREADS 6
#define ARRAY_SIZE 1000 //array_size is 10000
#define FILTERING_THREADS 3
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

const unsigned int V_HYPER_ID = {1, 2, 4};
const unsigned int V_PROD_ID = {32, 64, 128, 256, 512};

/*
    Lowest cost of 5 products average
*/

LowestCostHypermarket lowestcost;

/*
    Array of array of procuts
*/

ProductsFromHypermarket vec[NUM_OF_HYPERMARKETS] = {
    {.id_h = 1, .n_elements = 0}, //1st hm
    {.id_h = 2, .n_elements = 0}, //2nd hm
    {.id_h = 3, .n_elements = 0}  //3nd hm
};

pthread_mutex_t filtering[FILTERING_THREADS];
int main()
{

    return 0;
}