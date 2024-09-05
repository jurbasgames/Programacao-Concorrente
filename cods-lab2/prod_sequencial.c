#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Uso: %s <tamanho do vetor> <nome do arquivo>\n", argv[0]);
        return 1;
    }

    long int N = atoi(argv[1]);
    float elem;
    double soma = 0;
    int fator = 1;
    FILE *descritorArquivo;
    size_t ret;

    float *vetor1 = (float *)malloc(sizeof(float) * N);
    float *vetor2 = (float *)malloc(sizeof(float) * N);
    if (vetor1 == NULL || vetor2 == NULL)
    {
        printf("Erro na alocação de memória.\n");
        return 1;
    }

    srand(time(NULL));

    for (long int i = 0; i < N; i++)
    {
        elem = (rand() % MAX) / 3.0 * fator;
        vetor1[i] = elem;
        fator *= -1;
    }
    for (long int i = 0; i < N; i++)
    {
        elem = (rand() % MAX) / 3.0 * fator;
        vetor2[i] = elem;
        fator *= -1;
    }

    for (long int i = 0; i < N; i++)
    {
        soma += vetor1[i] * vetor2[i];
    }

    descritorArquivo = fopen(argv[2], "wb");

    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    ret = fwrite(&N, sizeof(long int), 1, descritorArquivo);
    ret = fwrite(vetor1, sizeof(float), N, descritorArquivo);
    ret = fwrite(vetor2, sizeof(float), N, descritorArquivo);
    ret = fwrite(&soma, sizeof(double), 1, descritorArquivo);

    fclose(descritorArquivo);
    free(vetor1);
    free(vetor2);

    return 0;
}
