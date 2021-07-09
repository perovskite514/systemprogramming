#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "bb.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int bb_buf[10];
int l = 0;
int r = 0;
int counter = 0;
int bb_get(void){

    pthread_mutex_lock(&mutex);
    while(counter == 0){
        pthread_cond_wait(&cond, &mutex);
    }

    int ans = bb_buf[r];
    r = (r + 1) % 10;
    counter--;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    return ans;
}

void bb_put(int i){

    pthread_mutex_lock(&mutex);
    while(counter == 10){
        pthread_cond_wait(&cond, &mutex);
    }
    bb_buf[l] = i;
    l = (l + 1) % 10;
    counter++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    return;
}

