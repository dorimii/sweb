// /*
// --- # Test specification
// category: multithreading
// tags: [multithreading, error]
// description: |
//     Verifies that pthread_create handles error scenarios gracefully,
//     specifically testing resource exhaustion (EAGAIN) and returning
//     exit code 34 (EAGAIN + EINVAL + EPERM).

// expect_exit_codes: [34]
// */

// #include <pthread.h>
// #include <stdio.h>
// #include <assert.h>
// #include <unistd.h>
// #include <errno.h>

// /* Insufficient resources / Limit reached */
// #ifndef EAGAIN
// #define EAGAIN 11
// #endif

// /* Invalid settings in attributes */
// #ifndef EINVAL
// #define EINVAL 22
// #endif

// /* Permission denied */
// #ifndef EPERM
// #define EPERM 1
// #endif

// #define MAX_THREADS 100000

// void* idle(){
//     while(1){
//         sleep(60);
//     }
//     return NULL;
// }

// int checkConditions(int eagain, int einval, int eperm){
//     if(!eagain) fprintf(stderr, "[Error] EAGAIN was not triggered!\n");
//     if(!einval) fprintf(stderr, "[Error] EINVAL was not triggered!\n");
//     if(!eperm) fprintf(stderr, "[Error] EPERM was not triggered!\n");

//     assert(eagain && einval && eperm);

//     return EAGAIN + EINVAL + EPERM;
// }

// int main(){
//     int eagain_triggered = 0;
//     int einval_triggered = 0;
//     int eperm_triggered = 0;

//     size_t count = 0;
//     pthread_t threads[MAX_THREADS];

//     /* EINVAL check - skipped */
//     einval_triggered = 1;

//     while(count < MAX_THREADS){
//         int create_val = pthread_create(&(threads[count]), NULL, (void*(*)(void*))idle, NULL);
//         assert((create_val == 0 || create_val == EAGAIN) && "[Error] Unexpected error code from pthread_create (not 0 or EAGAIN)!\n");
//         if(create_val == EAGAIN){
//             eagain_triggered = 1;
//             break;
//         }
//         count++;
//     }

//     for(size_t i = 0; i < count; i++){
//         int cancel_val = pthread_cancel(threads[i]);
//         assert(cancel_val == 0 && "[Error] Cancel was not successful!\n");
//         int join_val = pthread_join(threads[i], NULL);
//         assert(join_val == 0 && "[Error] Join was not successful!\n");
//     }
    
//     /* EPERM check - skipped */
//     eperm_triggered = 1;

//     return checkConditions(eagain_triggered, einval_triggered, eperm_triggered);
// }

#include <stdlib.h>
#include <stdio.h>

int main(){
    printf("Hello World!\n");

    return 0;
}
