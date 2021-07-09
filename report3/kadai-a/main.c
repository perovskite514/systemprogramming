#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include "btree.h"

void *func(void *arg){
    for(int i=0; i<2500; i++) btree_insert(i, arg);
    return 0;
}

int main() {

    pthread_t thread1, thread2, thread3, thread4;
    Tnode *tree;
    tree = btree_create();

    if(pthread_create(&thread1, NULL, func, tree) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_create(&thread2, NULL, func, tree) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_create(&thread3, NULL, func, tree) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_create(&thread4, NULL, func, tree) != 0){
        printf("error: pthread_create\n");
        return 1;
    }

    if(pthread_join(thread1, NULL) != 0){
        printf("error: pthread_join\n");
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

    btree_dump(tree); // output "1¥n1¥n2¥n"
    btree_destroy(tree);
    return 0;
    
}
