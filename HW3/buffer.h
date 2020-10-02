//this is going to have an array, two seampahores, and two ints (nextin and nextout)
// wdjwiejwiejiw
//
#include <pthread.h> 
#include <semaphore.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h>

typedef struct buffer{
	char myBuff[81];

	
	int nextIn;
	int nextOut;

	sem_t full;
	sem_t empty;
} buffer;




buffer* initializeBuff();
void deposit(buffer *b, char c);
char remoove(buffer *b);
void end();

