#ifndef PERSONAL_DATA_H
#define PERSONAL_DATA_H
#define PD_STR_SIZE 64
#define SHM_DB_SIZE 100
typedef struct
{
    int number;
    char name[PD_STR_SIZE];
    char address[PD_STR_SIZE];
} PersonalData;
#endif