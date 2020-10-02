#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

void One(int writeD);
void Two(int writeD, int readD);
void Three(int readD);

char e = EOF;
char ca = '^';
char s = ' ';

int main( int argc, const char* argv[] ){


	int pipe1[2];
	int pipe2[2];
	pid_t child1pid;
	pid_t child2pid;
	pid_t child3pid;
	//	child2pid, child3pid;

	if(pipe(pipe1) == -1 || pipe(pipe2) == -1){

		//error message
		printf("error");

		return 0;
	}

	/*	

		switch(child1pid = fork()){

		case -1:
		printf("error");

		case 0: //child
		close(pipe1[0]);
		printf("got before methodOne in switch");
		One(pipe1[1]);
		printf("got after method one in swithc");
		default:
		printf("got here in the switch");
		close(pipe1[1]);
		close(pipe2[0]);
		Two(pipe1[0], pipe2[1]);
		wait(NULL);
		}	
		*/	

	child1pid = fork();

	if(child1pid == 0){
		/*
		*/
		child2pid = fork();

		if(child2pid == 0){
//			printf("please second");
			close(pipe2[1]);

			Three(pipe2[0]);
			//wait(NULL);
			printf("got here too!");
			//return 0;
			kill(child2pid, SIGTERM);
			kill(child1pid, SIGTERM);
			exit(0);
			return 0;
		}else{

//			printf("got to second child");

			close(pipe1[1]);
			close(pipe2[0]);
			Two(pipe1[0], pipe2[1]);
			wait(NULL);

			/*
			*/
		}	
	}else{
		//		printf("got here first");	
		close(pipe1[0]);
		One(pipe1[1]);
		close(pipe1[1]);

		wait(NULL);			
	}

	//	return 0;

}



void Three(int readD){
	char c ;
	int cc = 0;
	int counter =0;
	char st[81];

	read(readD, &c, sizeof(char));
	//	printf("inside third method");
	while(c != EOF){

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

		read(readD, &c, sizeof(char));
	}
	printf("please get here EOF test");
	return;
	

	//kill(b2);

}

void One(int writeD){
	//	printf("got to one");	
	int c ;
	int cc = 0;
	char output[81] = "";

	do{
		c = getchar();
		if(c != EOF){
			write(writeD, &c, sizeof(char));
		}
	}while (c != EOF);

	/*while((c = getchar()) != EOF){	
	//deposit(b1, c);
	write(writeD, &c, sizeof(char));	
	}
	*/

	//deposit(b1, EOF);
	write(writeD, &e, sizeof(char));
	return;
}

void Two(int readD, int writeD){
	//	printf("inside second methods");

	int c;
	int cc = 0;

	read(readD, &c, sizeof(char));
	while(c != EOF){

		switch (c){
			case'\n':
				// output[strlen(output)] = ' ';

				//deposit(b2,' ');

				write(writeD, &s, sizeof(char));


				//deposit(b2, '\0');
				// output[strlen(output)+1] = '\0';
				break;
			case'*':{
					//int tempChar = getchar();


					//char tempC = remoove(b1);

					char tempC;	
					read(readD, &tempC, sizeof(char));
					if(tempC != EOF && tempC == '*'){
						//output[strlen(output)] = '^';

						//deposit(b2, '^');    
						write(writeD, &ca, sizeof(char));


						//deposit(b2, '^');
						//output[strlen(output)+1] = '\0';
						break;
					}else{
						//ungetc(tempChar, stdin);
						/*write(writeD, &tempC, sizeof(char));
						  if(tempC == EOF){
						  return;
						  }*/

						//deposit(b2, c);
						write(writeD, &c, sizeof(char));
						if(tempC == '\n'){
							//deposit(b2, ' ');
							write(writeD, &s, sizeof(char));
						}else{
							//deposit(b2, tempC);
							write(writeD, &tempC, sizeof(char));
						}
						if(tempC == EOF){
							//printf("got here");
							return;
						}
						break;


						//deposit(b1, tempC);
					}
				}
			default:
				//output[strlen(output)] = c;

				write(writeD, &c, sizeof(char));
				//deposit(b2, c);
				//output[strlen(output)+1] = '\0';
				break;
		}

		read(readD, &c, sizeof(char));

	}
	write(writeD, &e, sizeof(char));
	return;
	//deposit(b2, EOF); kill(b1);
}


