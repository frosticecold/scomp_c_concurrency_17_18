#include "sm.h"
#include "shared_data.h"

int main(){
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_WRITE);
    unlink(SHM_DELETE);
    addr=create_shared_memory(SHM_DEFAULT_FLAGS);
    memset(addr,0,sizeof(shared_memory));
    return 0;
}