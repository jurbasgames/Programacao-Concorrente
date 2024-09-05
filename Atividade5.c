#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int tamanho;
    int *vetor;
} args_t;
void *somaVetor(void *arg)
{
    args_t *args = (args_t *)arg;
    for (int i = 0; i < args->tamanho; i++)
    {
        args->vetor[i] += 1;
    }
}

int main(int argc, char *argv[])
{
    int M = atoi(argv[1]);
    pthread_t threads[M];
    int tamVetor = atoi(argv[2]);
    int vetor[tamVetor];
    for (int i = 0; i < tamVetor; i++)
    {
        vetor[i] = i;
    }
    printf("Vetor criado\n");

    // Divisão de carga

    int carga = tamVetor / M;
    int resto = tamVetor % M;

    for (int i = 0; i < M; i++)
    {
        args_t *arg = (args_t *)malloc(sizeof(args_t));
        arg->tamanho = carga;
        arg->vetor = vetor + i * carga;
        if (i == M - 1)
        {
            arg->tamanho += resto;
        }
        pthread_create(&threads[i], NULL, somaVetor, arg);
    }
    printf("Threads criadas\n");

    for (int i = 0; i < M; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Thread %d finalizada\n", i);
    }
    printf("Checando soma do vetor\n");
    for (int i = 0; i < tamVetor; i++)
    {
        printf("%d -- %d\n", vetor[i], i + 1);
        if (vetor[i] != i + 1)
        {
            printf("Erro na casa %d\n", i);
            return 1;
        }
    }
}
