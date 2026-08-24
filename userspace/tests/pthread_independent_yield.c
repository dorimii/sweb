#include <pthread.h>
#include <stdio.h>
#include <sched.h>

#define NUM_THREADS 3
#define ITERATIONS 20
//Verifies that multiple pthreads can run independently and yield to each other

volatile int progress[NUM_THREADS];

void* threadFunction(void* arg)
{
  int id = *(int*)arg;
  for(int i = 0; i < ITERATIONS; i++)
  {
    progress[id]++;
    sched_yield();
  }
  return NULL;
}

int main()
{
  pthread_t threads[NUM_THREADS];
  int ids[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++)
  {
    ids[i] = i;
    progress[i] = 0;
    if(pthread_create(&threads[i], NULL, threadFunction, &ids[i]) != 0)
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
    if(progress[i] != ITERATIONS)
    {
      printf("Test failed, thread %d progress expected %d, got %d\n", i, ITERATIONS, progress[i]);
      return 1;
    }
  }
  printf("Test passed, all threads ran independently\n");
  return 0;
}