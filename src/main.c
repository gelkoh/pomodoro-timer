#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void print_usage()
{
    printf("Usage: hours -h <hours> | minutes -m <minutes> | seconds -s <seconds>\n");
}

void *threadproc(void *total_seconds)
{
    int total_remaining_seconds = (*(int *)total_seconds);

    int remaining_hours = total_remaining_seconds / 60 / 60;
    int remaining_minutes = total_remaining_seconds / 60 % 60;
    int remaining_seconds = total_remaining_seconds % 60;

    while (total_remaining_seconds > 0)
    {
        printf("\e[2J\e[H");
        printf("Pomodoro:\n");
        printf("%02d:%02d:%02d\n", remaining_hours, remaining_minutes, remaining_seconds);

        total_remaining_seconds--;

        remaining_hours = total_remaining_seconds / 60 / 60;
        remaining_minutes = total_remaining_seconds / 60 % 60;
        remaining_seconds = total_remaining_seconds % 60;

        sleep(1);
    }

    return 0;
}

int main(int argc, char **argv)
{
    int option;
    int pomodoro_minutes = 0;

    while ((option = getopt(argc, argv, "p:b")) != -1)
    {
        char *end;

        switch(option)
        {
            case 'p':
                pomodoro_minutes = strtol(optarg, &end, 10);
                break;
            default:
                print_usage();
        }
    }

    int total_pomodoro_seconds = pomodoro_minutes * 60;

    pthread_t tid;
    pthread_create(&tid, NULL, &threadproc, (void *)&total_pomodoro_seconds);
    pthread_join(tid, NULL);

    return 0;
}
