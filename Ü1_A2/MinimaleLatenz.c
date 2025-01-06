#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <limits.h> // Für LONG_MAX

#define ITERATIONS 1000000

// Funktion, um die Zeit in Nanosekunden zu berechnen
long diff_in_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
}

int main() {
    struct timespec start, end;
    long min_latency = LONG_MAX;
    long max_latency = 0;
    long total_latency = 0;

    for (int i = 0; i < ITERATIONS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        getpid();
        clock_gettime(CLOCK_MONOTONIC, &end);

        long latency = diff_in_ns(start, end);

        // Minimal-, Maximal- und Gesamtlatenz aktualisieren
        if (latency < min_latency) {
            min_latency = latency;
        }
        if (latency > max_latency) {
            max_latency = latency;
        }
        total_latency += latency;
    }

    // Durchschnittliche Latenz berechnen
    double avg_latency = (double)total_latency / ITERATIONS;

    printf("Iterationen: %d\n", ITERATIONS);
    printf("Minimale Latenz: %ld ns\n", min_latency);
    printf("Maximale Latenz: %ld ns\n", max_latency);
    printf("Durchschnittliche Latenz: %.2f ns\n", avg_latency);

    // Messung für mehrere Aufrufe
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        getpid();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Durchschnittliche Latenz berechnen
    double new_avg_latency = (double)diff_in_ns(start, end) / ITERATIONS;

    printf("Durchschnittliche Latenz pro System-Call: %.2f ns\n", new_avg_latency);
}
