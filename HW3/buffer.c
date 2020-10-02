#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>        
#include "buffer.h"
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // sleep
#include <sys/types.h> // kill
//#include <signal.h> // kill
#define MESSAGE_LENGTH 80
#define ERROR -1
#define SLEEP_TIME 1


buffer* initializeBuff(){
	
//	int nextIn = 0;
//        int nextOut = 0;
//        char array[81] = calloc(81, sizeof(char));


	//These are the neccessary arguments for mmap. See man mmap.
 	void* addr = 0;
 	int protections = PROT_READ|PROT_WRITE; //can read and write
	int flags = MAP_SHARED|MAP_ANONYMOUS; //shared b/w procs & not mapped to a file
 	int fd = -1; //We could make it map to a file as well but here it is not needed.
	off_t offset = 0;

 	//Create memory map
 	buffer* myBuff = mmap(addr, sizeof(buffer), protections, flags, fd, offset);

 	if (( void *) ERROR == myBuff){//on an error mmap returns void* -1.
		perror("error with mmap");
		exit(EXIT_FAILURE);
	}
		
	//buffer *myBuff = calloc(1, sizeof(buffer));	
	
//	buffer *b = {
//		nextIn = nextIn,
//		nextOut = nextOut,
//		array = array

	
//	};
	
	myBuff-> nextIn = 0;
	myBuff-> nextOut = 0;
//	myBuff-> full = 0;
//	myBuff-> empty = 80;

	sem_init(&myBuff->empty, 1, 80);
	sem_init(&myBuff->full, 1, 0);
	return myBuff;

}


void deposit(buffer *b, char c){
	sem_wait(&b->empty);
	b->myBuff[b->nextIn] = c;
	b->nextIn = (b->nextIn+1)%80;
	//printf("got to deposit %s \n", b->myBuff);
	sem_post(&b->full);
}


char remoove(buffer *b){
	//printf("start of remove %s\n", b->myBuff);
	sem_wait(&b->full);
	
        char c = b->myBuff[b->nextOut];
	b->nextOut = (b->nextOut+1)%80;
        sem_post(&b->empty);
	return c;
}


void end(buffer *b){

	sem_destroy(&b->empty);
	sem_destroy(&b->full);
}
