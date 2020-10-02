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
  		if (getline(&input, &buf, stdin) < 0){
			return 0;
		}
		
		pid_t pid;
                int stat;
 	  // figure out EOF if()		
	  if((pid = fork()) < 0){
                // error code
                        fprintf(stderr, "ERROR\n");
                
	  }else if (pid == 0) {


		char *inPointer = input;

		arg = strtok(inPointer, " \t\r\n\a");
		int counter = 0;
		
		while(arg != NULL){
		
			args[counter] = arg;	
			//printf("'%s'\n", *&args[counter]);
			counter++;
			arg = strtok(NULL, " \t\r\n\a");
		}
		
		args[counter] = (char *)NULL;
		//if (strcmp(args[0], "exit") == 0)  
               	//	exit(0);

/*		pid_t pid;
		int stat;

		if((pid = fork()) < 0){
		// error code
			fprintf(stderr, "ERROR\n");
		}else if (pid == 0) {          */


			if (execvp(*args, args) < 0) {  
               			fprintf(stderr, "ERROR\n");
               			exit(1);
          			//return 1;
			}
		return 0;
	  }else {                              
          		stat = (wait(&pid));
     		}

	}
			
	return 0;
}
