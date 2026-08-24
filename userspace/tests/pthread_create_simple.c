#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg)
{
    (void)arg;
    printf("[INFO] function is being executed\n");
    return NULL;
}

int main()
{
    pthread_t thread;

    printf("[INFO] create thread\n");

    if (pthread_create(&thread, NULL, thread_function, NULL) == 0)
        printf("[PASS] thread created successfully\n");
    else
        printf("[FAIL] thread creation failed\n");

    return 0;
}