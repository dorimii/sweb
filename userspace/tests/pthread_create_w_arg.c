#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg)
{
    int value = *(int*)arg;

    printf("[INFO] thread received argument: %d\n", value);

    return arg;
}

int main()
{
    pthread_t thread;
    int value = 42;

    printf("[INFO] create thread\n");

    int result = pthread_create(&thread, NULL, thread_function, &value);

    printf("[INFO] pthread_create returned: %d\n", result);

    if (result == 0)
        printf("[PASS] thread created successfully\n");
    else
        printf("[FAIL] thread creation failed\n");

    return 0;
}