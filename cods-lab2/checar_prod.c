#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s <nome do arquivo>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "rb");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    long int N;
    fread(&N, sizeof(long int), 1, arquivo);

    float *vetor1 = (float *)malloc(sizeof(float) * N);
    float *vetor2 = (float *)malloc(sizeof(float) * N);
    if (vetor1 == NULL || vetor2 == NULL)
    {
        printf("Erro de alocação de memória.\n");
        fclose(arquivo);
        return 1;
    }

    fread(vetor1, sizeof(float), N, arquivo);
    fread(vetor2, sizeof(float), N, arquivo);

    double soma;
    fread(&soma, sizeof(double), 1, arquivo);

    printf("Tamanho do vetor: %ld\n", N);

    if (argc > 2 && strcmp(argv[2], "-v") == 0)
    {
        printf("Vetor 1: ");
        for (long int i = 0; i < N; i++)
        {
            printf("%f ", vetor1[i]);
        }
        printf("\n\n");

        printf("Vetor 2: ");
        for (long int i = 0; i < N; i++)
        {
            printf("%f ", vetor2[i]);
        }
        printf("\n\n");
    }

    printf("Produto interno: %lf\n", soma);

    fclose(arquivo);
    free(vetor1);
    free(vetor2);

    return 0;
}
