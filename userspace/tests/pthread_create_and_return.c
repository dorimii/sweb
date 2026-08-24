/*
--- # Test specification
category: multithreading
tags: [multithreading, success]
description: |
    Creates a thread, passes a number as an argument, and checks if
    pthread_join receives the same number back.

expect_exit_codes: [0]
*/

#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void* runner(void* arg){
    return arg;
}

int main(){
    size_t num = 0xBADC0D3;
    pthread_t thread;

    int create_val = pthread_create(&thread, NULL, (void*(*)(void*))runner, (void*)num);
    assert(create_val == 0 && "[Error] Thread was not successfully created!\n");

    void* ret_val;
    int join_val = pthread_join(thread, &ret_val);
    assert(join_val == 0 && "[Error] Pthread_join failed!\n");

    assert((size_t)ret_val == num && "[Error] Wrong return value!");

    return 0;
}
