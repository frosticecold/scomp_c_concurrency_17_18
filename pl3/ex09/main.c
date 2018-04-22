#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#define ARRAY_SIZE 10
#define LIMIT_BY_PROCESS 100
#define NUM_PROCESSES 10
#define BUFFER 512

typedef struct
{
    char path[100];
    char word[10];
    int numberOcorrences;
} WordPath;

void populateWordStructForSearch(WordPath *wordpath)
{
    int i = 0;
    char *word = "main()";
    for (; i < ARRAY_SIZE; i++)
    {
        strcpy(wordpath[i].word, word);
        sprintf(wordpath[i].path, "%s%d", "test/file", i + 1);
        wordpath[i].numberOcorrences = 0;
    }
}

int main()
{
    pid_t pid[NUM_PROCESSES];
    int status[NUM_PROCESSES];
    int i = 0;
    int fd, data_size = sizeof(WordPath); // definição de variavel  para tamhanho de memoria e para a criação de memoria partilhada
    WordPath *w;

    fd = shm_open("/ex09", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para escrita
    if (fd == -1)                                                         // verificação de criação de ficheiro de memoria partilhada para escrita
    {
        perror("Erro partilha de memoria Writer");
        return EXIT_FAILURE;
    }
    int memoryAmount = ftruncate(fd, data_size); // definição de memoria a ser partilhada
    if (memoryAmount == -1)                      // verificação de definição de memoria partilhada
    {
        perror("Erro a definir tamanho de memoria");
        return EXIT_FAILURE;
    }

    w = (WordPath *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "s" a memoria que foi partilhada

    if (w == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
        return EXIT_FAILURE;
    }

    /*===== Populate the struct with a path and a word to search =====*/

    populateWordStructForSearch(w);

    /*================================================================*/

    /*====================Process creation====================*/

    for (i = 0; i < NUM_PROCESSES; ++i)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            printf("Erro na criacao de processo");
            exit(-1);
        }
        if (pid[i] == 0) //if its child
        {
            /*====================Child Code====================*/
            FILE *newFile;
            char tempWord[BUFFER];
            newFile = fopen(w[i].path, "r");

            printf("%s", w[i].path);
            if (newFile == NULL)
            {
                perror("Error open file\n");
                exit(-1);
            }

            while (fgets(tempWord, 512, newFile) != NULL) // the file will be read while it has content, while its not end of file (EOF)
            {
                if ((strstr(tempWord, w[i].word)) != NULL) // strstr compares a substring in a line of the file
                {
                    printf("A match found on file :%s\n", w[i].path);
                    printf("\n%s\n", tempWord);
                    w[i].numberOcorrences++;
                }
            }

            fclose(newFile);

            /*====================Child Code====================*/
            exit(1);
        }
    }
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        waitpid(pid[i], &status[i], 0); // wait for all child to finish
    }
    int total = 0;
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (WIFEXITED(status[i])) // if all childs have ended
        {
            total += w[i].numberOcorrences;/* count the ocorrences for each child*/
        }
    }
    printf("The total number of occurrences for the word \"%s\" are %d\n", w[0].word, total);
    if (munmap(w, data_size) == -1)
    { //unmap de memoria com verificação de erro
        printf("Erro unmap\n");
    }
    if (close(fd) == -1)
    { //close de memoria com erificação de erro
        printf("Erro close\n");
    }
    if (shm_unlink("/ex09") == -1)
    { //shm_unlink de memoria para remoção de ficheiro de memoria parilhada com verificação de erro
        printf("Erro unlink\n");
    }

    return 0;
}