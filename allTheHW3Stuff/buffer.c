#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>        
#include "buffer.h"
#include <stdlib.h>


buffer* initializeBuff(){
	
//	int nextIn = 0;
//        int nextOut = 0;
//        char array[81] = calloc(81, sizeof(char));
	
	buffer *myBuff = calloc(1, sizeof(buffer));	
	
//	buffer *b = {
//		nextIn = nextIn,
//		nextOut = nextOut,
//		array = array

	
//	};
	
	myBuff-> nextIn = 0;
	myBuff-> nextOut = 0;
//	myBuff-> full = 0;
//	myBuff-> empty = 80;

	sem_init(&myBuff->empty, 0, 80);
	sem_init(&myBuff->full, 0, 0);
	return myBuff;

}


void deposit(buffer *b, char c){
	sem_wait(&b->empty);
	b->array[b->nextIn] = c;
	b->nextIn = (b->nextIn+1)%80;
	sem_post(&b->full);
}


char remoove(buffer *b){
	sem_wait(&b->full);
        char c = b->array[b->nextOut];
        b->nextOut = (b->nextOut+1)%80;
        sem_post(&b->empty);
	return c;
}


void kill(buffer *b){

	sem_destroy(&b->empty);
	sem_destroy(&b->full);
}
