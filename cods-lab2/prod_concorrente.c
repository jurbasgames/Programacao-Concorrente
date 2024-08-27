#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long int N;
float *vetor1;
float *vetor2;
int nthreads;

void *ProdutoInterno(void *tid)
{
    long int id = (long int)tid;
    int ini, fim, bloco;
    double *soma_local;

    bloco = N / nthreads;
    ini = id * bloco;
    fim = ini + bloco;
    if (id == (nthreads - 1))
        fim = N;

    soma_local = malloc(sizeof(double));
    if (soma_local == NULL)
    {
        printf("--ERRO: malloc() thread\n");
        pthread_exit(NULL);
    }

    *soma_local = 0;
    for (int i = ini; i < fim; i++)
    {
        *soma_local += vetor1[i] * vetor2[i];
    }

    pthread_exit((void *)soma_local);
}

int main(int argc, char *argv[])
{
    FILE *file;
    size_t ret;

    double prod_arq;
    double prod_conc;
    double *ret_threads;

    pthread_t *tid_sistema;

    if (argc < 3)
    {
        printf("Use: %s <nome do arquivo> <numero de threads> \n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("--ERRO: fopen()\n");
        return 1;
    }

    ret = fread(&N, sizeof(long int), 1, file);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes do vetor no arquivo\n");
        return 3;
    }

    vetor1 = malloc(sizeof(float) * N);
    vetor2 = malloc(sizeof(float) * N);
    if (vetor1 == NULL || vetor2 == NULL)
    {
        printf("--ERRO: malloc()\n");
        return 1;
    }
    ret = fread(vetor1, sizeof(float), N, file);
    ret = fread(vetor2, sizeof(float), N, file);
    if (ret < N)
    {
        fprintf(stderr, "Erro de leitura dos elementos dos vetores\n");
        return 4;
    }

    nthreads = atoi(argv[2]);

    tid_sistema = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL)
    {
        printf("--ERRO: malloc()\n");
        return 1;
    }

    // Chama as threads
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_create(&tid_sistema[i], NULL, ProdutoInterno, (void *)i))
        {
            printf("--ERRO: pthread_create()\n");
            return 1;
        }
    }

    // Produto interno das threads
    prod_conc = 0;
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_join(tid_sistema[i], (void **)&ret_threads))
        {
            printf("--ERRO: pthread_join()\n");
            return 1;
        }
        prod_conc += *ret_threads;
        free(ret_threads);
    }

    ret = fread(&prod_arq, sizeof(double), 1, file);
    printf("Produto interno sequencial:  %.26f\n", prod_arq);
    printf("Produto interno concorrente: %.26f\n", prod_conc);
    double erro_relativo = fabs(prod_arq - prod_conc) / fabs(prod_arq);
    printf("Erro relativo:               %.26f\n", erro_relativo);

    free(vetor1);
    free(vetor2);
    free(tid_sistema);
    fclose(file);
    return 0;
}
