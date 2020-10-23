/*
 * Authors: Anderson Henrique Giacomini - 769720
 * Gustavo Vieira Jodar - 769678
 * Lucas Cruz dos Reis(L.C.R.) - 754757
 * Sophia S Schuster - 760936
 * Tiago Lucas Leal - 760937
 * Desc.: A program to solve The Modus Hall Problem for the subject of
 * Operational Systems(SO) during the second semester of 2020.
 * I: <entry>
 * O: <output>
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

int heathens_counter = 0;
int prudes_counter = 0;
int prudes_turn = 0;
int heathens_turn = 0;

int prudes_done = 0;
int heathens_done = 0;

int NUM_PRUDES = 2;
int NUM_HEATHENS = 2;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void prudes(void)
{
    pthread_mutex_lock(&mutex);

    prudes_counter++;

    printf("\n%sPRUDES ARRIVED%s\nPrudes: %d\nHeathens: %d",
           "\033[31;1m", "\033[00m",
           prudes_counter, heathens_counter);
    pthread_mutex_unlock(&mutex);

    prudes_turn--;
    while (prudes_turn == 0)
    {
    }
    pthread_mutex_lock(&mutex);

    prudes_counter--;
    if (!prudes_counter && heathens_counter)
    {
        heathens_turn++;
    }
    if (heathens_counter > prudes_counter)
    {
        heathens_turn++;
    }
    else
    {
        prudes_turn++;
    }
    prudes_done++;
    printf("\n%sPRUDES FINISHED%s\nPrudes: %d\nHeathens: %d",
           "\033[31;1m", "\033[00m",
           prudes_counter, heathens_counter);
    printf("\nRemaining:\n");
    printf("Heathens: %d\n", NUM_HEATHENS - heathens_done);
    printf("Prudes: %d\n", NUM_PRUDES - prudes_done);

    pthread_mutex_unlock(&mutex);
}

void heathens(void)
{
    pthread_mutex_lock(&mutex);

    heathens_counter++;

    printf("\n%sHEATHENS ARRIVED%s\nPrudes: %d\nHeathens: %d",
           "\033[31;1m", "\033[00m",
           prudes_counter, heathens_counter);
    pthread_mutex_unlock(&mutex);

    heathens_turn--;
    while (heathens_turn == 0)
    {
    }

    pthread_mutex_lock(&mutex);

    heathens_counter--;
    if (!heathens_counter && prudes_counter)
    {
        prudes_turn++;
    }
    if (prudes_counter > heathens_counter)
    {
        prudes_turn++;
    }
    else
    {
        heathens_turn++;
    }
    heathens_done++;
    printf("\n%sHEATHENS FINISHED%s\nPrudes: %d\nHeathens: %d",
           "\033[31;1m", "\033[00m",
           prudes_counter, heathens_counter);
    printf("\nremaining:\n");
    printf("Heathens: %d\n", NUM_HEATHENS - heathens_done);
    printf("Prudes: %d\n", NUM_PRUDES - prudes_done);
    pthread_mutex_unlock(&mutex);
}

void run()
{
    int i;
    int num_heathens, num_prudes, start;
    pthread_t *tid_prudes, *tid_heathens;

    num_heathens = NUM_HEATHENS;
    num_prudes = NUM_PRUDES;

    printf("-- Initially --\n");
    printf("Prudes: %d\n", num_prudes);
    printf("Heathens: %d\n", num_heathens);

    if (num_heathens > num_prudes)
        start = 1;
    else
        start = 0;

    tid_heathens = malloc(num_heathens * sizeof(pthread_t));
    tid_prudes = malloc(num_prudes * sizeof(pthread_t));

    if (num_heathens >= num_prudes)
    {
        for (i = 0; i < num_prudes; i++)
        {
            pthread_create(&tid_prudes[i], NULL, (void *(*)(void *)) & heathens, NULL);
            if (i < num_prudes)
                pthread_create(&tid_prudes[i], NULL, (void *(*)(void *)) & prudes, NULL);
        }

        for (i = 0; i < num_heathens; i++)
        {
            pthread_join(tid_heathens[i], NULL);
            if (i < num_prudes)
                pthread_join(tid_prudes[i], NULL);
        }
    }
    else
    {
        for (i = 0; i < num_prudes; i++)
        {
            pthread_create(&tid_prudes[i], NULL, (void *(*)(void *)) & prudes, NULL);
            if (i < num_heathens)
                pthread_create(&tid_heathens[i], NULL, (void *(*)(void *)) & heathens, NULL);
        }

        for (i = 0; i < num_prudes; i++)
        {
            pthread_join(tid_prudes[i], NULL);
            if (i < num_heathens)
                pthread_join(tid_heathens[i], NULL);
        }
    }
}

int main(int argc, char *argv[])
{
    run();

    return 0;
}
