#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <pthread.h>

// #define DEBUG

typedef struct
{
    int linhas;
    int colunas;
    float *elementos;
} Matriz;

typedef struct
{
    int dim;
    int indiceInicial;
    int indiceFinal;

    Matriz *matrizA;
    Matriz *matrizB;
    Matriz *resultado;
} tArgs;

void *tarefa(tArgs *arg)
{
    tArgs *args = (tArgs *)arg;
    for (int i = args->indiceInicial; i < args->indiceFinal; i++)
    {
        int linha = i / args->resultado->colunas;
        int coluna = i % args->resultado->colunas;
        float soma = 0;
        for (int j = 0; j < args->dim; j++)
        {
            float a = args->matrizA->elementos[linha * args->dim + j];
            float b = args->matrizB->elementos[j * args->resultado->colunas + coluna];
            soma += a * b;
        }
        args->resultado->elementos[i] = soma;
    }
}

Matriz *lerMatriz(char *nomeArquivo)

{
    FILE *descritorArquivo = fopen(nomeArquivo, "rb");
    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return NULL;
    }

    Matriz *matriz = malloc(sizeof(Matriz));
    if (!matriz)
    {
        fprintf(stderr, "Falha na alocação de memória para a matriz\n");
        fclose(descritorArquivo);
        return NULL;
    }

    int ret = fread(&matriz->linhas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensões da matriz no arquivo\n");
        free(matriz);
        fclose(descritorArquivo);
        return NULL;
    }

    ret = fread(&matriz->colunas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensões da matriz no arquivo\n");
        free(matriz);
        fclose(descritorArquivo);
        return NULL;
    }

    int tamanho = matriz->linhas * matriz->colunas;
    matriz->elementos = (float *)malloc(sizeof(float) * tamanho);
    if (!matriz->elementos)
    {
        fprintf(stderr, "Falha na alocação de memória para os elementos da matriz\n");
        free(matriz);
        fclose(descritorArquivo);
        return NULL;
    }

    if (fread(matriz->elementos, sizeof(float), tamanho, descritorArquivo) < tamanho)
    {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        free(matriz->elementos);
        free(matriz);
        fclose(descritorArquivo);
        return NULL;
    }

    fclose(descritorArquivo);
    return matriz;
}

void multiplicarMatrizesConc(Matriz *matrizA, Matriz *matrizB, Matriz *resultado, int nthreads)
{
    resultado->linhas = matrizA->linhas;
    resultado->colunas = matrizB->colunas;
    int tamanhoRes = resultado->linhas * resultado->colunas;
    resultado->elementos = (float *)malloc(sizeof(float) * tamanhoRes);

    if (!resultado->elementos)
    {
        fprintf(stderr, "Erro de alocação de memória para os elementos da matriz resultado\n");
        return;
    }

    if (nthreads > tamanhoRes)
        nthreads = tamanhoRes;

    pthread_t *tid;
    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL)
    {
        puts("ERRO--malloc");
        return;
    }

    tArgs *args;
    args = (tArgs *)malloc(sizeof(tArgs) * nthreads);
    if (args == NULL)
    {
        puts("ERRO--malloc");
        return;
    }

    int carga = tamanhoRes / nthreads;
    int resto = tamanhoRes % nthreads;

    for (int i = 0; i < nthreads; i++)
    {
        args[i].dim = resultado->linhas;
        args[i].indiceInicial = i * carga;
        args[i].indiceFinal = (i + 1) * carga;
        args[i].matrizA = matrizA;
        args[i].matrizB = matrizB;
        args[i].resultado = resultado;
        if (i == nthreads - 1)
            args[i].indiceFinal += resto;
        if (pthread_create(&tid[i], NULL, tarefa, (void *)&args[i]))
        {
            puts("ERRO--pthread_create");
            return;
        }
    }

    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(tid[i], NULL);
    }
}

void escreverMatriz(char *nomeArquivo, Matriz *matriz)
{
    FILE *descritorArquivo;
    descritorArquivo = fopen(nomeArquivo, "wb");
    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return;
    }

    int retA, retB;
    retA = fwrite(&matriz->linhas, sizeof(int), 1, descritorArquivo);
    retB = fwrite(&matriz->colunas, sizeof(int), 1, descritorArquivo);
    if (!retA || !retB)
    {
        fprintf(stderr, "Erro de escrita das dimensoes da matriz no arquivo\n");
        fclose(descritorArquivo);
        return;
    }
    int tamanho = matriz->linhas * matriz->colunas;
    retA = fwrite(matriz->elementos, sizeof(float), tamanho, descritorArquivo);
    if (retA < tamanho)
    {
        fprintf(stderr, "Erro de escrita dos elementos da matriz\n");
        fclose(descritorArquivo);
        return;
    }
    fclose(descritorArquivo);
}

void freeMatriz(Matriz *matriz)
{
    free(matriz->elementos);
    free(matriz);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo matriz B> <numero de threads>\n", argv[0]);
        return 1;
    }

    double start, finish, elapsed = 0;
    GET_TIME(start);

    Matriz *matrizA, *matrizB, *resultado;

    char *arquivoA = argv[1];
    char *arquivoB = argv[2];
    int nthreads = atoi(argv[3]);

    // Ler as matrizes de arquivos binários
    matrizA = lerMatriz(arquivoA);
    matrizB = lerMatriz(arquivoB);

    if (matrizA == NULL || matrizB == NULL)
    {
        fprintf(stderr, "Erro de leitura das matrizes\n");
        return 2;
    }
    if (matrizA->colunas != matrizB->linhas)
    {
        fprintf(stderr, "Matrizes incompatíveis para multiplicação\n");
        return 3;
    }

    GET_TIME(finish);
    elapsed = finish - start;
    printf("Inicialização demorou %e segundos\n", elapsed);

    // Multiplicar as matrizes
    GET_TIME(start);
    resultado = malloc(sizeof(Matriz));
    multiplicarMatrizesConc(matrizA, matrizB, resultado, nthreads);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Multiplicação demorou %e segundos\n", elapsed);

    // Escrever a matriz resultante em um arquivo binário
    GET_TIME(start);

    char *arquivoResultado = "resultadoConc.bin";
    escreverMatriz(arquivoResultado, resultado);

    GET_TIME(finish);
    elapsed = finish - start;
    printf("Escrita demorou %e segundos\n", elapsed);

    // Liberar memória
    freeMatriz(matrizA);
    freeMatriz(matrizB);
    freeMatriz(resultado);

    return 0;
}