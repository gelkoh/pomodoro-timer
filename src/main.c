#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum Interval {
    POMODORO,
    SHORT_BREAK,
    LONG_BREAK
};

struct UserInput {
    int pomodoro_duration;
    int short_break_duration;
    int long_break_duration;
};

struct TimerData {
    int pomodoro_duration;
    int short_break_duration;
    int long_break_duration;
    enum Interval current_interval;
};

void print_usage()
{
    printf("Usage: hours -h <hours> | minutes -m <minutes> | seconds -s <seconds>\n");
}

struct UserInput get_user_input(int argc, char **argv)
{
    int option;
    int pomodoro_duration = 0;
    int short_break_duration = 0;
    int long_break_duration = 0;

    while ((option = getopt(argc, argv, "p:s:l:")) != -1)
    {
        char *end;

        switch(option)
        {
            case 'p':
                pomodoro_duration = strtol(optarg, &end, 10);
                break;
            case 's':
                short_break_duration = strtol(optarg, &end, 10);
                break;
            case 'l':
                long_break_duration = strtol(optarg, &end, 10);
                break;
            default:
                print_usage();
        }
    }

    struct UserInput user_input = {
        pomodoro_duration,
        short_break_duration,
        long_break_duration
    };

    return user_input;
}

void *threadproc(void *timer_data)
{
    struct TimerData *td = (struct TimerData *)timer_data;

    int total_remaining_seconds;
    int remaining_hours;
    int remaining_minutes;
    int remaining_seconds;

    int pomodoro_count = 0;

    while (true)
    {
        switch(td->current_interval) {
            case POMODORO:
                total_remaining_seconds = td->pomodoro_duration * 60;
                pomodoro_count++;
                break;
            case SHORT_BREAK:
                total_remaining_seconds = td->short_break_duration * 60;
                break;
            case LONG_BREAK:
                total_remaining_seconds = td->long_break_duration * 60;
        }

        while (total_remaining_seconds >= 0)
        {
            printf("\e[2J\e[H");

            switch(td->current_interval) {
                case POMODORO:
                    printf("Pomodoro:\n");
                    break;
                case SHORT_BREAK:
                    printf("Short break:\n");
                    break;
                case LONG_BREAK:
                    printf("Long break:\n");
                    break;
            }

            remaining_hours = total_remaining_seconds / 60 / 60;
            remaining_minutes = total_remaining_seconds / 60 % 60;
            remaining_seconds = total_remaining_seconds % 60;

            printf("%02d:%02d:%02d\n", remaining_hours, remaining_minutes, remaining_seconds);

            total_remaining_seconds--;

            sleep(1);
        }

        if (pomodoro_count % 4 == 0)
        {
            td->current_interval = LONG_BREAK;
        }
        else if (td->current_interval == POMODORO)
        {
            td->current_interval = SHORT_BREAK;
        }
        else
        {
            td->current_interval = POMODORO;
        }
    }

    return 0;
}

void start_timer(struct TimerData *timer_data)
{
    pthread_t tid;
    pthread_create(&tid, NULL, &threadproc, (void *)timer_data);
    pthread_join(tid, NULL);
}

int main(int argc, char **argv)
{
    struct UserInput user_input = get_user_input(argc, argv);

    struct TimerData timer_data = {
        user_input.pomodoro_duration,
        user_input.short_break_duration,
        user_input.long_break_duration,
        POMODORO
    };

    start_timer(&timer_data);

    return 0;
}
