#include <pthread.h>
#include <stdio.h>
#include <sched.h>

#define NUM_THREADS 4

//Verifies that all pthreads in the same process share and update global memory

volatile int shared_counter = 0;

void* threadFunction(void* arg)
{
  (void)arg;
  shared_counter++;
  sched_yield();
  return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++)
    {
      if(pthread_create(&threads[i], NULL, threadFunction, NULL) != 0)
      {
        printf("pthread_create failed for thread %d\n", i);
        return 1;
      }
    }

    for(int i = 0; i < 100000; i++)
    {
      sched_yield();
    }
    if(shared_counter == NUM_THREADS)
    {
      printf("All threads updated shared global counter\n");
    }
    else
    {
      printf("Test failed, expected counter %d, got %d\n", NUM_THREADS, shared_counter);
    }
    return 0;
}