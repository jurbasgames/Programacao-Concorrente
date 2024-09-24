#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

int buff[N];      // Estrutura compartilhada
int contador = 0; // Quantidade de elementos
int in = 0;       // Posição insere
int out = 0;      // Posição retira

pthread_mutex_t lock;
pthread_cond_t cond;

void inserir(int item);
int retirar();

int main(void)
{
main:
    return 0;
}

// Insere elemento no espaço compartilhado

void inserir(int item)
{
    pthread_mutex_lock(&lock);
    if (contador == N)
    {
        pthread_cond_wait(&cond, &lock);
    }
    buff[in] = item;
    contador++;
    in = (in + 1) % N;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
}

int retirar()
{
    int item;
    pthread_mutex_lock(&lock);
    if (!contador)
    {
        pthread_cond_wait(&cond, &lock);
    }
    item = buff[out];
    contador--;
    out = (out + 1) % N;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
    return item;
}