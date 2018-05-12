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

#define TOTAL_TRAINS 4
#define THREADS TOTAL_TRAINS

#define TRAIN_A 1
#define TRAIN_B 2
#define TRAIN_C 3
#define TRAIN_D 4

#define NUMBER_OF_TRAINS TOTAL_TRAINS
#define NUMBER_OF_CITIES 4

#define CITY_A "City A"
#define CITY_B "City B"
#define CITY_C "City C"
#define CITY_D "City D"

#define CITY_A_ID 1
#define CITY_B_ID 2
#define CITY_C_ID 3
#define CITY_D_ID 4

typedef struct
{
    int train_number;
    int destination;
    int from;
    char *city_destination;
    char *city_from;
} Train;

/*GLOBAL VARIABLES*/

Train train[TOTAL_TRAINS];
pthread_mutex_t mutex_city_A_to_B;
pthread_mutex_t mutex_city_B_to_C;
pthread_mutex_t mutex_city_B_to_D;

/*----------------*/

void generate_trains();

void initialize_varibles();

char *get_city_name(int);

pthread_mutex_t get_path(Train *);

char *get_time()
{
    char *time_str = (char *)malloc(sizeof(char) * 20);
    time_t now = time(NULL);
    strftime(time_str, 20, "%H:%M:%S", localtime(&now));
    return time_str;
}

/*----------------*/

void *func_thread(void *args)
{
    Train *train = (Train *)args;
    char *name_from = get_city_name(train->from);
    char *name_destination = get_city_name(train->destination);
    printf("Trip for train %d starts at: %s\n\n", train->train_number, get_time());
    printf("Train number: %d, is traveling from %s, to %s\n", train->train_number, name_from, name_destination);
    pthread_mutex_t first_trip_mutex = get_path(train); //get_path will get the correct mutex and update the from from struct train to then chek his destination
    if (pthread_mutex_lock(&first_trip_mutex) == -1)
    { // lock the train line to this train
        perror("mutex lock error");
    }

    printf("First trip train number %d has finished\n", train->train_number);
    sleep(2);
    if (pthread_mutex_unlock(&first_trip_mutex) == -1)
    { // unlock the train line for other trains
        perror("mutex unlock error");
    }

    if (train->from == train->destination) // if the destination is the same as from when updated from method get_path
    {                                      // it doesnt need to make another trip, so thread exits
        printf("Train number %d has finished his traveling\n", train->train_number);

        sleep(1);
        printf("Trip for train %d ended at: %s\n\n", train->train_number, get_time());
        pthread_exit(NULL);
    }
    name_from = get_city_name(train->from);
    name_destination = get_city_name(train->destination);

    printf("Train number: %d, is traveling from %s, to %s\n", train->train_number, name_from, name_destination);
    pthread_mutex_t second_trip_mutex = get_path(train); //if doesnt arrived in on trip it needs a second trip

    if (pthread_mutex_lock(&second_trip_mutex) == -1)
    { // lock the train line to this train
        perror("mutex unlock error");
    }
    printf("Second trip train number %d has finished\n", train->train_number);
    sleep(2);
    if (pthread_mutex_unlock(&second_trip_mutex) == -1)
    { // unlock the train line for other trains
        perror("mutex unlock error");
    }
    printf("Train number %d has finished traveling\n", train->train_number);
    sleep(1);
    printf("Trip for train %d ended at: %s\n\n", train->train_number, get_time());
    sleep(1);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[THREADS];
    initialize_varibles();
    int i = 0;
    for (; i < THREADS; i++)
    {
        /*====Creating threads==================*/
        if (pthread_create(&threads[i], NULL, func_thread, (void *)&train[i]) != 0)
        { //confirming error for creating thread
            perror("error creating thread\n");
        }
    }
    /*====Waiting for the threads==============*/
    for (i = 0; i < THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error pthread join\n");
        }
    }
    /*====Threads terminated===================*/
    return 0;
}

void generate_trains()
{
    /*====Generation of multiple trains========*/
    srand(time(NULL));
    int i = 0;
    for (; i < TOTAL_TRAINS; i++)
    {
        train[i].train_number = i + 1;
        train[i].destination = (rand() % NUMBER_OF_CITIES) + 1;
        train[i].from = (rand() % NUMBER_OF_CITIES) + 1;
        while (train[i].destination == train[i].from)
        {
            train[i].destination = (rand() % NUMBER_OF_CITIES) + 1;
            train[i].from = (rand() % NUMBER_OF_CITIES) + 1;
        }
    }
}

void initialize_varibles()
{
    generate_trains();
    if (pthread_mutex_init(&mutex_city_A_to_B, NULL) != 0) //initalize mutexes
    {
        perror("Error mutex creation A to B\n");
    }
    if (pthread_mutex_init(&mutex_city_B_to_C, NULL) != 0) //initalize mutexes
    {
        perror("Error mutex creation B to C\n");
    }
    if (pthread_mutex_init(&mutex_city_B_to_D, NULL) != 0) //initalize mutexes
    {
        perror("Error mutex creation B to D\n");
    }
}

pthread_mutex_t get_path(Train *train)
{
    switch (train->from)
    {
    case CITY_A_ID:
        train->from = CITY_B_ID; // update the location from to check if arrived its destination
        return mutex_city_A_to_B;
        break;
    case CITY_B_ID:
        switch (train->destination)
        {
        case CITY_A_ID:
            train->from = CITY_A_ID; // update the location from to check if arrived its destination
            return mutex_city_A_to_B;
            break;
        case CITY_C_ID:
            train->from = CITY_C_ID; // update the location from to check if arrived its destination
            return mutex_city_B_to_C;
            break;
        case CITY_D_ID:
            train->from = CITY_D_ID; // update the location from to check if arrived its destination
            return mutex_city_B_to_D;
            break;
        }
        break;
    case CITY_C_ID:
        train->from = CITY_B_ID;
        return mutex_city_B_to_C;
        break;
    case CITY_D_ID:
        train->from = CITY_B_ID;
        return mutex_city_B_to_D;
        break;
    }
    pthread_mutex_t empty; // to make sure it sends something
    return empty;
}

char *get_city_name(int city)
{
    switch (city)
    {
    case CITY_A_ID:
        return CITY_A;
        break;
    case CITY_B_ID:
        return CITY_B;
        break;
    case CITY_C_ID:
        return CITY_C;
        break;
    case CITY_D_ID:
        return CITY_D;
        break;
    }
    return "No city Avaliable";
}