#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define NUM_PHILOSOPHERS 7
pthread_mutex_t forks[NUM_PHILOSOPHERS];
enum PhilosopherState { THINKING, HUNGRY, EATING };
enum PhilosopherState philosopherStates[NUM_PHILOSOPHERS];

void pick_forks(int philosopher_id) {
    // Pick up the two forks
    pthread_mutex_lock(&forks[philosopher_id]);
    pthread_mutex_lock(&forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);
}

void release_forks(int philosopher_id) {
    // Put down the two forks
    pthread_mutex_unlock(&forks[philosopher_id]);
    pthread_mutex_unlock(&forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);
}

void* philosopher(void* arg) {
    int philosopher_id = *(int*)arg;
    srand(time(NULL));

    while (true) {
        // Philosopher thinks for a while
        philosopherStates[philosopher_id] = THINKING;
        printf("Philosopher %d is thinking\n", philosopher_id);
        usleep(rand() % 900000 + 100000);

        // Philosopher gets hungry and tries to pick up forks
        philosopherStates[philosopher_id] = HUNGRY;
        printf("Philosopher %d is hungry\n", philosopher_id);
        pick_forks(philosopher_id);

        // Philosopher eats for a while
        philosopherStates[philosopher_id] = EATING;
        printf("Philosopher %d is eating\n", philosopher_id);
        usleep(rand() % 900000 + 100000);

        // Philosopher is done eating, puts down forks and starts over
        printf("Philosopher %d is done eating\n", philosopher_id);
        release_forks(philosopher_id);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        philosopherStates[i] = THINKING;
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    while (true) {
        // Print the current state of each philosopher
        printf("Current philosophers states: ");
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            if (philosopherStates[i] == THINKING) {
                printf("T ");
            }
            else if (philosopherStates[i] == HUNGRY) {
                printf("H ");
            }
            else {
                printf("E ");
            }
        }
        printf("\n");

        // Wait for a while before printing the states again
        usleep(1000000);
    }

    return 0;
}
