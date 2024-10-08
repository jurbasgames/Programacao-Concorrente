#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int escritor_fila, escritores, leitores = 0;
pthread_cond_t cond_leitura, cond_escrita;

void read_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(&mutex);

    while (escritores > 0 || escritor_fila > 0)
    {
        pthread_cond_wait(&cond_leitura, &mutex);
    }
    leitores++;
    pthread_mutex_unlock(&mutex);
}

void rwrite_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(&mutex);
    escritor_fila++;

    while (leitores > 0 || escritores > 0)
    {
        pthread_cond_wait(&cond_leitura, &mutex);
    }
    escritor_fila--;
    escritores++;
    pthread_mutex_unlock(&mutex);
}

void read_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(&mutex);
    leitores--;
    if (leitores == 0)
    {
        pthread_cond_signal(&cond_escrita);
    }
    pthread_mutex_unlock(&mutex);
}

void write_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(&mutex);
    escritores--;
    if (escritor_fila > 0)
    {
        pthread_cond_signal(&cond_escrita);
    }
    else
    {
        pthread_cond_broadcast(&cond_leitura);
    }
    pthread_mutex_unlock(&mutex);
}
