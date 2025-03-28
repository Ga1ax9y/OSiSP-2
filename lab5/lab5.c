#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <linux/time.h>

#define MAX_PHILOSOPHERS 10

int num_philosophers = 5;
pthread_mutex_t *forks;
int eating_time = 2;
int thinking_time = 3;
int timeout_seconds = 2;

typedef struct {
    int id;
    long total_eating_time;
    long total_thinking_time;
    int conflict_count;
} Philosopher;

Philosopher philosophers[MAX_PHILOSOPHERS];

void random_delay(int min, int max) {
    int delay = min + rand() % (max - min + 1);
    usleep(delay * 1000000);
}

void print_status(int id, const char *action) {
    printf("Философ %d: %s\n", id, action);
    fflush(stdout);
}

int try_lock_with_timeout(pthread_mutex_t *mutex) {
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += timeout_seconds;
    return pthread_mutex_timedlock(mutex, &timeout);
}

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left_fork = id;
    int right_fork = (id + 1) % num_philosophers;

    while (1) {
        print_status(id, "размышляет");
        philosophers[id].total_thinking_time += thinking_time;
        sleep(thinking_time);

        print_status(id, "пытается взять вилки");

        if (try_lock_with_timeout(&forks[left_fork]) != 0) {
            philosophers[id].conflict_count++;
            continue;
        }

        if (try_lock_with_timeout(&forks[right_fork]) != 0) {
            pthread_mutex_unlock(&forks[left_fork]);
            philosophers[id].conflict_count++;
            continue;
        }

        print_status(id, "ест");
        philosophers[id].total_eating_time += eating_time;
        sleep(eating_time);

        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);
    }

    return NULL;
}

void print_statistics() {
    printf("\n=== СТАТИСТИКА ===\n");
    for (int i = 0; i < num_philosophers; i++) {
        printf("Философ %d:\n", i);
        printf("  Всего времени еды: %ld секунд\n", philosophers[i].total_eating_time);
        printf("  Всего времени размышлений: %ld секунд\n", philosophers[i].total_thinking_time);
        printf("  Количество конфликтов: %d\n", philosophers[i].conflict_count);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        num_philosophers = atoi(argv[1]);
        if (num_philosophers > MAX_PHILOSOPHERS || num_philosophers <= 0) {
            printf("Неверное количество философов. Допустимый диапазон: 1-%d\n", MAX_PHILOSOPHERS);
            return 1;
        }
    }

    srand(time(NULL));

    forks = malloc(num_philosophers * sizeof(pthread_mutex_t));
    for (int i = 0; i < num_philosophers; i++) {
        pthread_mutex_init(&forks[i], NULL);
        philosophers[i].total_eating_time = 0;
        philosophers[i].total_thinking_time = 0;
        philosophers[i].conflict_count = 0;
    }

    pthread_t threads[num_philosophers];
    int ids[num_philosophers];
    for (int i = 0; i < num_philosophers; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    sleep(20);

    for (int i = 0; i < num_philosophers; i++) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < num_philosophers; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    free(forks);

    print_statistics();

    return 0;
}
