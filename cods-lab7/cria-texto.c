#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char buffer[256];
    char ch;
    FILE *file;
    file = fopen("input.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file.");
    }
    for (int i = 21; i < 127; i++)
    {
        ch = i;
        buffer[i] = ch;
        fwrite(buffer, 1, i, file);
    }
    fclose(file);

    return 0;
}