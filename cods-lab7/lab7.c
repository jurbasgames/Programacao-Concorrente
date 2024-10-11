#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5

void *thread1();
void *thread2();
void *thread3();

char buff1[N], buff2[N];
int in1, out1, in2, out2 = 0;
sem_t vazio1, cheio1, vazio2, cheio2;
FILE *file;
int main(int argc, char *argv[])
{

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Erro abrindo arquivo");
        exit(1);
    }

    sem_init(&vazio1, 0, N);
    sem_init(&cheio1, 0, 0);
    sem_init(&vazio2, 0, N);
    sem_init(&cheio2, 0, 0);

    pthread_t tid[3];

    pthread_create(&tid[0], NULL, thread1, NULL);
    pthread_create(&tid[1], NULL, thread2, NULL);
    pthread_create(&tid[2], NULL, thread3, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    sem_destroy(&vazio1);
    sem_destroy(&cheio1);
    sem_destroy(&vazio2);
    sem_destroy(&cheio2);
    return 0;
}

void *thread1()
{
    char ch;
    while (1)
    {
        ch = fgetc(file);
        if (ch == EOF)
        {
            break;
        }

        sem_wait(&vazio1);
        buff1[in1] = ch;
        in1 = (in1 + 1) % N;
        sem_post(&cheio1);
    }
    pthread_exit(NULL);
}
void *thread2()
{
    int caracteres = 0;
    int n = 0;
    while (1)
    {
        sem_wait(&cheio1);
        char elem = buff1[out1];
        out1 = (out1 + 1) % N;
        sem_post(&vazio1);

        sem_wait(&vazio2);
        buff2[in2] = elem;
        in2 = (in2 + 1) % N;
        sem_post(&cheio2);
        caracteres++;

        if (caracteres % (2 * n + 1) == 0)
        {
            sem_wait(&vazio2);
            buff2[in2] = '\n';
            in2 = (in2 + 1) % N;
            caracteres = 0;
            sem_post(&cheio2);
            if (n <= 9)
                n++;
        }
    }
}
void *thread3()
{
    while (1)
    {
        sem_wait(&cheio2);
        char elem = buff2[out2];
        out2 = (out2 + 1) % N;
        sem_post(&vazio2);
        printf("%c", elem);
        fflush(stdout);
    }
}