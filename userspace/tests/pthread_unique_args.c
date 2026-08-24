#include <pthread.h>
#include <stdio.h>
#include <sched.h>

#define NUM_THREADS 5
//Verifies that each pthread receives the correct unique argument passed at creation

int args[NUM_THREADS];
volatile int results[NUM_THREADS];

void* threadFunction(void* arg)
{
  int id = *(int*)arg;
  results[id] = id + 100;
  sched_yield();
  return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++)
    {
      args[i] = i;
      results[i] = -1;
      if(pthread_create(&threads[i], NULL, threadFunction, &args[i]) != 0)
      {
        printf("pthread_create failed for thread %d\n", i);
        return 1;
      }
    }

    for(int i = 0; i < 100000; i++)
    {
      sched_yield();
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
      if(results[i] != i + 100)
      {
        printf("Test failed, result[%d] expected %d, got %d\n", i, i + 100, results[i]);
        return 1;
      }
    }
    printf("Test passed, each thread received and used its own argument\n");
    return 0;
}