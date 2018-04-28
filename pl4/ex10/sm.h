#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>         //perror
#include <string.h>        //memset
#include "personal_data.h" //Personal_data struct
#define SHM_FILENAME "/pl4_ex10"
#define SHM_DELETE "/dev/shm" SHM_FILENAME
#define DB_SIZE 100
/** shared memory struct */
typedef struct
{
    int fd;
    int readcount;
    int writecount;
    int index;
    PersonalData db[DB_SIZE];
} shared_memory;

shared_memory *create_shared_memory(int flags);
void delete_shared_memory(shared_memory *addr);
void check_if_shm_exists();
#endif