#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define N 5

char buff[N];

sem_t sem1, sem2;
FILE *file;
int main(int argc, char *argv[])
{

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Erro abrindo arquivo");
    }

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);

    pthread_t tid[3];

    pthread_create(&tid[0], NULL, thread1, NULL);
    pthread_create(&tid[1], NULL, thread2, NULL);
    pthread_create(&tid[2], NULL, thread3, NULL);
}

void *thread1()
{
    sem_wait(&sem1);
    while (fread(buff, sizeof(char), 1, file) != EOF)
    {
        sem_post(&sem2);
    }
}
void *thread2();
void *thread3();