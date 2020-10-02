#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( int argc, const char* argv[] ){
	
	//char *input[1000];
	char *arg;
	char **args;

	while(1){
		printf("%% "); 
	
		char *input = NULL;
  		ssize_t buf = 4000; 
  		getline(&input, &buf, stdin);

		char *inPointer = input;

		arg = strtok(inPointer, " \t\r\n\a");
		int counter = 0;
		
		while(arg != NULL){
		
			args[counter] = arg;	
			//printf("'%s'\n", *&args[counter]);
			counter++;
			arg = strtok(NULL, " \t\r\n\a");
		}
		
		args[counter] = NULL;

		if (strcmp(args[0], "exit") == 0)  
               		exit(0);

		pid_t pid;
		int stat;

		if((pid = fork()) < 0){
		// error code
		}else if (pid == 0) {          
          		
			if (execvp(*args, args) < 0) {  
               			//error message printf("ERROR\n");
               			exit(1);
          	
			}

		}else {                              
          		while (wait(&stat) != pid);
     		}

	}
			
	
}
