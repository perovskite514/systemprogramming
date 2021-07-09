#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "btree.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Tnode * btree_create(void){

    Tnode *r = (Tnode*)malloc(sizeof(Tnode));
    r->data = 1;
    r->flag = 0;
    r->right = NULL;
    r->left = NULL;
    return r;
    
}

void btree_insert(int v, Tnode *t){

    pthread_mutex_lock(&mutex);

    Tnode *r = (Tnode*)malloc(sizeof(Tnode));
    r->data = v;
    r->flag = 1;
    r->right = NULL;
    r->left = NULL;

    if(t->flag == 0){
        t->flag = 1;
        t->data = v;
        pthread_mutex_unlock(&mutex);
        return;
    }

    while(1){

        int u = t->data;
        
        if(t->left == NULL){
            t->left = r;
            break;
        } 

        if(t->right == NULL){
            t->right = r;
            break;
        }

        if(v <= u){
            t = t->left;
        }

        else{
            t = t->right;
        }

    }

    pthread_mutex_unlock(&mutex);
    return;
}

void btree_destroy(Tnode *t){

    if(t == NULL){
        free(t);
        return;
    }
    btree_destroy(t->left);
    btree_destroy(t->right);
    free(t);
    return;
}

void btree_dump(Tnode *t){

    if(t == NULL){
        return;
    }

    btree_dump(t->left);
    printf("%d\n", t->data);
    btree_dump(t->right);
    return;
}
