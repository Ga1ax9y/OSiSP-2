#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// pgrep lab4  kill -9 <>
#define COUNTER_FILE "counter.txt"

volatile int counter = 0;

void signal_handler(int signum) {
    printf("Signal %d received. Creating a child process...\n", signum);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {

        printf("Child process created with PID %d\n", getpid());
        return;
    } else {
        printf("Parent process with PID %d is exiting.\n", getpid());
        exit(EXIT_SUCCESS);
    }
}

void write_counter_to_file() {
    FILE *file = fopen(COUNTER_FILE, "w");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    fprintf(file, "%d\n", counter);
    fclose(file);
}

int main() {
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    printf("Process started with PID %d\n", getpid());

    while (1) {
        sleep(1);
        counter++;
        printf("Counter: %d\n", counter);
        write_counter_to_file();
    }

    return 0;
}
