#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>  //perror
#include <string.h> //memset

#define SHM_FILENAME "/pl4_ex08"
#define SHM_DELETE "/dev/shm" SHM_FILENAME

/** shared memory struct */
typedef struct
{
    int fd;
    int numC;
    int numS;
} shared_memory;

shared_memory *create_shared_memory(void);
void delete_shared_memory(shared_memory *addr);
void check_if_shm_exists();
#endif