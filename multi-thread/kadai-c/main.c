#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include "bb.h"
#include <assert.h>

void *func2(){
    for(int i=0; i<5000; i++) bb_put(i);
    return 0;
}

void *func3(){
    for(int i=0; i<5000; i++){
        printf("%d\n", bb_get());
    }
    return 0;
}

int main() {

    pthread_t thread2, thread3, thread4, thread5;

    for(int i=0; i<10; i++) bb_put(i);
    for(int i=0; i<10; i++){
        assert(bb_get() == i);
    }

    if(pthread_create(&thread2, NULL, func2, NULL) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_create(&thread3, NULL, func2, NULL) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_create(&thread4, NULL, func3, NULL) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_create(&thread5, NULL, func3, NULL) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_join(thread2, NULL) != 0){
        printf("error: pthread_join\n");
        return 1;
    }

    if(pthread_join(thread3, NULL) != 0){
        printf("error: pthread_join\n");
        return 1;
    }

    if(pthread_join(thread4, NULL) != 0){
        printf("error: pthread_join\n");
        return 1;
    }

    if(pthread_join(thread5, NULL) != 0){
        printf("error: pthread_join\n");
        return 1;
    }

    return 0;

}
