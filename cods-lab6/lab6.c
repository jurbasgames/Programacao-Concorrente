#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int escritor_fila, escritores, leitores = 0;
pthread_cond_t cond_leitura, cond_escrita;

void read_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);

    if (escritores > 0 || escritor_fila > 0)
    {
        printf("Leitor bloqueado. Escritores ativos ou na fila.\n"); // No if, para printar uma vez apenas
    }

    while (escritores > 0 || escritor_fila > 0)
    {
        pthread_cond_wait(&cond_leitura, mutex);
    }

    leitores++;
    pthread_mutex_unlock(mutex);
}

void write_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);

    escritor_fila++;

    if (leitores > 0 || escritores > 0)
    {
        printf("Escritor esperando leitores finalizarem.\n");
    }

    while (leitores > 0 || escritores > 0)
    {
        pthread_cond_wait(&cond_escrita, mutex);
    }

    escritor_fila--;
    escritores++;

    printf("Escritor começou a escrever. Escritores ativos: %d\n", escritores);

    pthread_mutex_unlock(mutex);
}

void rw_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);

    if (leitores > 0)
    {
        leitores--;
        if (leitores == 0)
        {
            printf("Último leitor acabou, liberando escritores.\n");
            pthread_cond_signal(&cond_escrita);
        }
    }
    else if (escritores > 0)
    {
        escritores--;
        printf("Escritor acabou de escrever.\n");
        if (escritor_fila > 0)
        {
            printf("Acordando próximo escritor.\n");
            pthread_cond_signal(&cond_escrita);
        }
        else
        {
            printf("Não há mais escritores na fila, liberando leitores.\n");
            pthread_cond_broadcast(&cond_leitura);
        }
    }
    pthread_mutex_unlock(mutex);
}