#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

// #define DEBUG

typedef struct
{
    int linhas;
    int colunas;
    float *elementos;
} Matriz;

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

void multiplicarMatrizes(Matriz *matrizA, Matriz *matrizB, Matriz *resultado)
{

    resultado->elementos = (float *)malloc(sizeof(float) * matrizA->linhas * matrizB->colunas);
    if (!resultado->elementos)
    {
        fprintf(stderr, "Falha na alocação de memória para os elementos da matriz resultado\n");
        return NULL;
    }

    resultado->linhas = matrizA->linhas;
    resultado->colunas = matrizB->colunas;

    for (int i = 0; i < matrizA->linhas; i++)
    {
        for (int j = 0; j < matrizB->colunas; j++)
        {
            float soma = 0;
            for (int k = 0; k < matrizA->colunas; k++)
            {
                soma += matrizA->elementos[i * matrizA->colunas + k] * matrizB->elementos[k * matrizB->colunas + j];
            }
            resultado->elementos[i * resultado->colunas + j] = soma;
        }
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
    if (argc < 3)
    {
        fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo matriz B>\n", argv[0]);
        return 1;
    }

    double start, finish, elapsed = 0;
    GET_TIME(start);

    Matriz *matrizA, *matrizB, *resultado;

    char *arquivoA = argv[1];
    char *arquivoB = argv[2];

    // Ler as matrizes de arquivos binários
    matrizA = lerMatriz(arquivoA);
    matrizB = lerMatriz(arquivoB);

    if (matrizA == NULL || matrizB == NULL)
    {
        fprintf(stderr, "Erro de leitura das matrizes\n");
        return 2;
    }

    GET_TIME(finish);
    elapsed = finish - start;
    printf("Inicialização demorou %e segundos\n", elapsed);

    // Multiplicar as matrizes
    GET_TIME(start);
    resultado = malloc(sizeof(Matriz));
    multiplicarMatrizes(matrizA, matrizB, resultado);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Multiplicação demorou %e segundos\n", elapsed);

    // Escrever a matriz resultante em um arquivo binário
    GET_TIME(start);

    char *arquivoResultado = "resultadoSeq.bin";
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