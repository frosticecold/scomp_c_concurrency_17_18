#include "sm.h"
shared_memory *create_shared_memory(void)
{
    int fd;
    const char *const workingdir = SHM_FILENAME;
    int flags = O_CREAT | O_EXCL | O_RDWR;
    mode_t mode = S_IRUSR | S_IWUSR;
    fd = shm_open(workingdir, flags, mode);
    if (fd == -1)
    {
        perror("Erro ao criar memoria partilhada.");
        return NULL;
    }
    if (ftruncate(fd, sizeof(shared_memory)) == -1)
    {
        perror("Erro ao truncar a memória.");
        return NULL;
    }
    int prot_flags = PROT_READ | PROT_WRITE;
    shared_memory *addr = mmap(NULL, sizeof(shared_memory), prot_flags, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Erro ao fazer map à memória.");
        return NULL;
    }
    memset(&addr[0], 0, sizeof(shared_memory));
    addr->fd = fd;
    return addr;
}
void delete_shared_memory(shared_memory *addr)
{
    int fd = addr->fd;
    munmap(addr, sizeof(shared_memory));
    close(fd);
    if (shm_unlink(SHM_FILENAME) == -1)
        perror("Erro ao fazer shm unlink");
}
void check_if_shm_exists()
{

    if (access(SHM_DELETE, F_OK) == 0)
    {
        if (unlink(SHM_DELETE) < 0)
            perror("Erro ao apagar.");
    }
}