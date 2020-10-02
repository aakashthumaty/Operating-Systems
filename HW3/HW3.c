#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "buffer.h"
#include <stdlib.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

buffer *b1, *b2;

void threadOne();
void threadTwo();
void threadThree();


int main( int argc, const char* argv[] ){

	b1 = initializeBuff();
	b2 = initializeBuff();

	pid_t pid, pid1, pid2; 
  
	
	
	pid = fork(); 
    	if(pid == 0){
		threadOne();
		return 0;
	}
	pid1 = fork();
	if (pid1 == 0){
		threadTwo();
		return 0;
	}
	pid2 = fork();
	if(pid2 == 0){

		threadThree();
		return 0;
	}
	wait((int *) 0);	
	wait((int *) 0);
	wait((int *) 0);
	
	return 0;

	
/*	
	pid1 = fork();
	if (pid1 == 0){
		//printf("3");
		threadThree();
		//return 0;
		//wait((int *) 0);
	
	}else{
		printf("2\n");
		threadTwo();
		wait((int *) 0);
	}
    }else{
	printf("1");
	threadOne();
	wait((int *) 0);
    }
   
    return 0;

    */
}


void threadThree(){
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

	end(b2);
	return;
}

void threadOne(){
        int c ;
        int cc = 0;
        char output[81] = "";

        while(c != EOF){
                c = getchar();
		if(c != EOF){
                        //write(writeD, &c, sizeof(char));
                	deposit(b1, c);
		}
        }
	if(c == EOF){
		//printf("shouldnt get here\n");
	}
	deposit(b1, EOF);
	
	//return;
}

void threadTwo(){

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
                                	deposit(b2, c);
					if(tempC == '\n'){
						deposit(b2, ' ');
					}else{
						deposit(b2, tempC);
					}
					if(tempC == EOF){
						return;
					}
					break;
				}
                        }
                        default:
                                //output[strlen(output)] = c;
				
				deposit(b2, c);
				//output[strlen(output)+1] = '\0';
                                break;
                }
		
				

	}
	//printf("skipped loop\n");
	deposit(b2, EOF); end(b1);
	return;
}



