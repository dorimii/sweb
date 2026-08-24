/*
--- # Test specification
category: multithreading
tags: [multithreading, success]
description: |
    Tests that a thread can modify a shared variable via a pointer and
    that pthread_cancel correctly stops the sleeping thread with
    exit code PTHREAD_CANCELED.

expect_exit_codes: [0]
*/

#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#ifndef PTHREAD_CANCELED
#define PTHREAD_CANCELED (void*)-1
#endif

void* runner(void* ptr){
    while(1){
        *(size_t*)ptr = 0xDEADBEEF;
        sleep(15);
        assert(0 && "[Error] Thread was not canceled!\n");
    }
    return NULL;
}

int main(){
    size_t num = 0xBADC0DE;
    pthread_t thread;

    int create_val = pthread_create(&thread, NULL, (void*(*)(void*))runner, (void*)&(num));
    assert(create_val == 0 && "[Error] Thread was not successfully created!\n");

    int cancel_val = pthread_cancel(thread);
    assert(cancel_val == 0 && "[Error] Pthread_cancel failed!\n");

    void* ret_val;
    int join_val = pthread_join(thread, &ret_val);
    assert(join_val == 0 && "[Error] Pthread_join failed!\n");
    
    assert(num == 0xDEADBEEF && "[Error] Stack number was not correctly overwritten!\n");
    assert(ret_val == PTHREAD_CANCELED && "[Error] Thread did not return PTHREAD_CANCELED!\n");

    return 0;
}
