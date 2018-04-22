#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h> //perror
#include <string.h> //memset
#define STR_SIZE 80
#define NUM_OF_STRIGS 50
#define SHM_FILENAME "/pl4_ex03"
#define SHM_DELETE "/dev/shm" SHM_FILENAME

/** shared memory struct */
typedef struct
{
    int fd;
    char str[50][80];
    int index;
} shared_memory;

shared_memory *create_shared_memory(void);
void delete_shared_memory(shared_memory *addr);
void check_if_shm_exists();
#endif