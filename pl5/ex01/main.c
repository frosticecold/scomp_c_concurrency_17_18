/*

int main() {
    pid_t pid=fork();

    if (pid == 0)\{
        fork();
        pthread thread_id;
        pthread_create (&thread_id,NULL, thread_func, NULL);
        pthread_join(thread_id,NULL);
    }
    fork();
    ...
}




*/