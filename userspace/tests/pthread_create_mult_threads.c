#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 10

void* thread_function(void* arg)
{
    (void)arg;
    printf("[INFO] function is being executed\n");
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int errors = 0;

    printf("[INFO] create %d threads\n", NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0)
        {
            printf("[FAIL] thread %d creation failed\n", i);
            ++errors;
        }
    }

    if (errors == 0)
        printf("[PASS] all threads created successfully\n");
    else
        printf("[FAIL] %d threads could not be created\n", errors);

    return 0;
}