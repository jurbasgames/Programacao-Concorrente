#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct
{
    int i;
} typedef tArgs;

void ThreadFunc(tArgs args)
{
    static int contador = 0;
    pthread_mutex_lock(&mutex);
}

int main(int argc, char *argv[])
{

    void *tid = malloc(sizeof(void *) * 5);
    for (int i = 0; i < 5; i++)
    {
        tArgs args;
        args.i = i;
        pthread_create(tid, NULL, ThreadFunc, (void *)&args);
    }
}