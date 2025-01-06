#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define ITERATIONS 1000000 

int pipe_fd[2];  

void* thread_function(void* arg) {
    char buffer;
    for (int i = 0; i < ITERATIONS; i++) {
        // Lesen von der Pipe, Kontextwechsel zu Thread 1
        read(pipe_fd[0], &buffer, 1);
        // Schreiben in die Pipe, Kontextwechsel zu Hauptthread
        write(pipe_fd[1], &buffer, 1);
    }
    return NULL;
}

int main() {
    struct timespec start, end;
    pthread_t thread;
    char buffer = 'x';

    // Pipe erstellen
    if (pipe(pipe_fd) != 0) {
        perror("Pipe konnte nicht erstellt werden");
        return 1;
    }

    // Thread erstellen
    if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
        perror("Thread konnte nicht erstellt werden");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < ITERATIONS; i++) {
        // Schreiben in die Pipe, Kontextwechsel zu Thread 2
        write(pipe_fd[1], &buffer, 1);
        // Lesen von der Pipe, Kontextwechsel zurück
        read(pipe_fd[0], &buffer, 1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    pthread_join(thread, NULL);

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Zeit in Nanosekunden berechnen
    long total_time_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    double avg_context_switch_time = total_time_ns / (double)(2 * ITERATIONS);

    printf("Durchschnittliche Kontextwechselzeit: %.2f ns\n", avg_context_switch_time);
    return 0;
}
