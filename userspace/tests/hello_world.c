#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

void* runner(void* arg){
    printf("It's me the runner!\n");
    return arg;
}

int main(){
    printf("Hello World!\n");

    size_t num = 0xBADC0D3;
    pthread_t thread;

    int create_val = pthread_create(&thread, NULL, (void*(*)(void*))runner, (void*)num);
    assert(create_val == 0 && "[Error] Thread was not successfully created!\n");

    for(size_t i = 0; i < 100000000; i++){}

    return 0;
}
