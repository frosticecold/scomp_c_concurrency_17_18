#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>  //perror
#include <string.h> //memset
#define SHM_FILENAME "/pl4_ex12"
#define SHM_DELETE "/dev/shm" SHM_FILENAME
#define SHM_DEFAULT_FLAGS O_CREAT | O_RDWR
#define MAX_CLIENTS 5

/** shared memory struct */
typedef struct
{
    int fd;
    int numClientes;
    int bilhetes[MAX_CLIENTS];
    int nr_bilhete;
} shared_memory;

shared_memory *create_shared_memory(int flags);
void delete_shared_memory(shared_memory *addr);
void check_if_shm_exists();
#endif