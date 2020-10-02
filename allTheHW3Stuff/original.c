#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "buffer.h"
#include <stdlib.h>

buffer *b1, *b2, *b3;

void *threadOne();
void *threadTwo();
void *threadThree();


int main( int argc, const char* argv[] ){

	b1 = initializeBuff();
	b2 = initializeBuff();

	pthread_t tOne, tTwo, tThree;
	pthread_create(&tOne, NULL, threadOne, NULL); 
	pthread_create(&tTwo, NULL, threadTwo, NULL);
	pthread_create(&tThree, NULL, threadThree, NULL);
	
	pthread_join(tOne, NULL); 
	pthread_join(tTwo, NULL);	
	pthread_join(tThree, NULL);	


	return 0;
}


void *threadThree(){
        char c ;
        int cc = 0;
	int counter =0;
	char st[81];

	while((c = remoove(b2)) != EOF){
                
		st[counter] = c;
		counter++;
		if(counter  == 80){
                        for(int i = 0; i < 80; i++){
				printf("%c", st[i]);
			}
			printf("\n");
                        //memset(output, 0, 81);
                	counter = 0;
		}

	}

}

void *threadOne(){
	
        int c ;
        int cc = 0;
        char output[81] = "";

        while((c = getchar()) != EOF){	
		deposit(b1, c);
			
	}deposit(b1, EOF);
}

void *threadTwo(){


	int c;
	int cc = 0;

	while((c = remoove(b1)) != EOF){
		
		switch (c){
                        case'\n':
                               // output[strlen(output)] = ' ';
				deposit(b2,' ');
				//deposit(b2, '\0');	
			        // output[strlen(output)+1] = '\0';
				break;
                        case'*':{
                                //int tempChar = getchar();
				char tempC = remoove(b1);
                                if(tempC != EOF && tempC == '*'){
                                        //output[strlen(output)] = '^';
					deposit(b2, '^');    
					//deposit(b2, '^');
					//output[strlen(output)+1] = '\0';
                                        break;
                                }else{
                                        //ungetc(tempChar, stdin);
                                	deposit(b2, tempC);
				}
                        }
                        default:
                                //output[strlen(output)] = c;
				
				deposit(b2, c);
				//output[strlen(output)+1] = '\0';
                                break;
                }
		
				

	}deposit(b2, EOF);
}



