#include <pthread.h>

#define NUM_OF_PRODUCTS 5
#define NUM_OF_HYPERMARKETS 3
#define NUM_OF_CHARS 3

typedef struct
{
    char id_h[NUM_OF_CHARS];
    char id_p[NUM_OF_CHARS];
    float p_price;
} product;