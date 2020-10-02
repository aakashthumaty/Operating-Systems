/*
 * A server program that provides the service of converting a string
 * to upper case.  It is implemented using the "daemon-service"
 * model.  In this model, multiple clients can be serviced
 * concurrently by the service.  The server main process is
 * a simple loop that accepts incoming socket connections, and for
 * each new connection established, uses fork() to create a child
 * process that is a new instance of the service process.  This child
 * process will provide the service for the single client program that
 * established the socket connection.
 *
 * Each new instance of the server process accepts input strings from
 * its client program, converts the characters to upper case and returns
 * the converted string back to the client.
 *
 * Since the main process (the daemon) is intended to be continuously
 * available, it has no defined termination condition and must be
 * terminated by an external action (Ctrl-C or kill command).
 *
 * The server has one parameter: the port number that will be used
 * for the "welcoming" socket where the client makes a connection.
 */

#define MAX_LINE 50000
#define MAX_TMP 100
#define MAX_ARGS 4  /* allows program name + 3 positional parameters */
#define MIN_ARGS 2  /* must have at least 1 positional parameter */
#define NUM_PARMS 4  /* allows program name + 2 parameters + NULL */
#define ERR_RETURN -1

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Socket.h"
//#include "ToUpper.h" /* definitions shared by client and server */

/* variables to hold socket descriptors */
ServerSocket welcome_socket;
Socket connect_socket;

void toupper_service(void);

int main(int argc, char *argv[]) {
	pid_t spid, term_pid; /* pid_t is typedef for Linux process ID */
	int chld_status;
	bool forever = true;
	char *parms[50000];
//	printf("got her");
	if (argc < 2) {
		printf("No port specified\n");
		return (-1);
	}

	/* create a "welcoming" socket at the specified port */
	welcome_socket = ServerSocket_new(atoi(argv[1]));
	if (welcome_socket < 0) {
		printf("Failed new server socket\n");
		return (-1);
	}

	/* The daemon infinite loop begins here; terminate with external action*/
	//while (forever) {
		/* accept an incoming client connection; blocks the
		 * process until a connection attempt by a client.
		 * creates a new data transfer socket.
		 */
		connect_socket = ServerSocket_accept(welcome_socket);
		if (connect_socket < 0) {
			printf("Failed accept on server socket\n");
			exit(-1);
		}
		spid = fork(); /* create child == service process */
		if (spid == -1) {
			perror("fork");
			exit(-1);
		}
		if (spid == 0) { /* code for the service process */
			int i;
			/*
			   for (i = 0; i < (argc - 1); i++)
			   parms[i] = argv[i];
			   parms[i] = NULL;
			   printf("parms array: %s", parms[0]);
			   */

			toupper_service();
			

			Socket_close(connect_socket);
			exit(0);
		}    /* end service process */
		else /* daemon process closes its connect socket */
		{
			Socket_close(connect_socket);
			/* reap a zombie every time through the loop, avoid blocking*/
			term_pid = waitpid(-1, &chld_status,0);
			return 0;
		}
	//} /* end of infinite loop for daemon process */
	//return 0;
}


void toupper_service(void){

	pid_t cpid, term_pid, namePID; /* type for Linux process id */
	int chld_status;
	int i, c;
	int rc;
	int count = 0;
	//char *parms_array[NUM_PARMS];  /* arrary of pointers to strings */
	unsigned char new_line[MAX_LINE];
	unsigned char tmp_name[MAX_TMP];
	unsigned char id_str[MAX_TMP];
	int id;

	char *parms_array[NUM_PARMS];
	char line_data[50000];
	char *arg;
	bool foreva = true;
	/* variable names for file "handles" */
	FILE *tmpFP;
	FILE *fp;
	bool execFailed = false;
	Socket_close(welcome_socket);

	while (foreva){


		//fprintf(stderr, "%% please");
		/*
		   char *input = NULL;
		   ssize_t buf = 4000;
		   if (getline(&input, &buf, stdin) < 0){
		   return 0;
		   }
		   */
		for (i = 0; i < 50000; i++) {
			c = Socket_getc(connect_socket);
			if (c == EOF) {
				fprintf(stderr, "THIS ONE EOF EOF EOFSocket_getc EOF or error\n");
				foreva = false;
				line_data[i] = EOF;
				return; //break;
				//return; /* assume socket EOF ends service for this client */
			} else {
				if (c == '\0') {
                                        line_data[i] = c;
                                        break;
                                }
				//fprintf(stderr, "inputting into shell command \n");
				line_data[i] = c;
			}
		}

		/* validity check parameter count */
		/*
		   if ((argc > MAX_ARGS) ||
		   (argc < MIN_ARGS))
		   {
		   printf("Bad parameter count %d\n", argc);
		   printf("Positional parameters: executable name, [parm 1 [parm 2]]\n");
		   exit (ERR_RETURN);
		   }
		   */
		//char* p = &a[0] 
		char* inPointer = &line_data[0];
		//fprintf(stderr, "inPOinter: %s", inPointer);
		arg = strtok(inPointer, " \t\r\n\a");
		int counter = 0;

		while(arg != NULL){

			parms_array[counter] = arg;
			//fprintf(stderr, "'%s'\n", *&parms_array[counter]);
			counter++;
			arg = strtok(NULL, " \t\r\n\a");
		}

		parms_array[counter] = (char *)NULL;
		/*
		for(int ps = 0; ps < counter; ps++){
			fprintf(stderr, "this is parms array %s", parms_array[ps]);
			
		}
		*/
		
		namePID = getpid();
		sprintf(id_str, "%d", namePID);
		strcpy(tmp_name,"tmp");
		strcat(tmp_name, id_str);

		//fprintf(stderr, "tmp_name: %s\n", tmp_name);
		/* create child process, report error if present and exit */
		cpid = fork();
		if (cpid == -1)
		{
			perror("fork"); /* perror creates description of OS error codes */
			exit(ERR_RETURN);
		}

		if (cpid == 0)
		{ /* the child process */
			/* dynamically redirect stdout to the named temporary 
			 * file open for writing
			 */
			fp = freopen(tmp_name, "w", stdout);

			/* do the OS execvp() call, if there is an error return, report it.  Otherwise
			 * this call will not return and the new image will execute as this process ID.
			 */
			//fprintf(stderr, "calling exec with: %s\n", parms_array[0]);
			rc = execvp(parms_array[0], parms_array);
			if (rc == -1)
			{	
				//int ugh = 0;
				//ugh = Socket_putc((char)'f', connect_socket);
				printf("exec failed\n");
				//execFailed = true;
				//fprintf(stderr, "exec failed darn it");
				perror("execvp");
				exit(-1);
			}
		}  /* end of code that can be executed by the child process */

		else
		{/* code executed by the parent process */
			/* wait for the child to terminate and determine its exit status */
			/* this is necessary to prevent the accumulation of "Zombie" processes */
			
			term_pid = waitpid(cpid, &chld_status, 0);
			
			if (term_pid == -1)
			{	perror("waitpid");
				//term_pid = Socket_putc((char)'f', connect_socket);
			}
			else
			{
				if (WIFEXITED(chld_status)){
					printf("PID %d exited, status = %d\n", cpid, WEXITSTATUS(chld_status));
				}else{
					 //rc = Socket_putc((char)('f'), connect_socket);
                               		 //rc = Socket_putc('\0', connect_socket);
					printf("PID %d did not exit normally\n", cpid);
			
				}
			}

			if ((tmpFP = fopen (tmp_name, "r")) == NULL) {
				fprintf (stderr, "error opening tmp file\n");
				exit (-1);
			}
			while (!feof (tmpFP)) {
				/* Get line from file */
				
				if (fgets (new_line, sizeof(new_line), tmpFP) == NULL)
					break;
				count = strlen(new_line) ; /* include '\0' in count */
				for (i = 0; i < count; i++) {
					c = new_line[i];
					rc = Socket_putc((char)new_line[i], connect_socket);
					//fprintf(stderr, "%c", new_line[i]);
					if (execFailed){
						rc = Socket_putc((char)'f', connect_socket);
					}
					if (rc == EOF) {
						
						//fprintf(stderr, "FOUND AN EOFEOFEOFEOFEOFEOF");
						fprintf(stderr, " EOF EOF EOF Socket_putc EOF or error\n");
						kill(cpid, SIGKILL);
						return; /* assume socket EOF ends service for this client */
					}
				}

			}
			/* delete the temporary file */
			rc = Socket_putc('\0', connect_socket);
			remove(tmp_name);
			
			printf("Parent process terminating.\n");
		} /* end of code that can be executed by the parent process */
		//exit(0);
		//return;
	}
	//kill(cpid, SIGKILL);
	return;	
}


void nopetoupper_service(void) {
	int i, c, rc;
	int count = 0;
	bool forever = true;
	char line_data[MAX_LINE];

	/* will not use the server socket */
	Socket_close(welcome_socket);

	while (forever) /* actually, until EOF on connect socket */
	{
		/* get a null-terminated string from the client on the data
		 * transfer socket up to the maximim input line size.  Continue
		 * getting strings from the client until EOF on the socket.
		 */
		for (i = 0; i < MAX_LINE; i++) {
			c = Socket_getc(connect_socket);
			if (c == EOF) {
				printf("Socket_getc EOF or error\n");
				return; /* assume socket EOF ends service for this client */
			} else {
				if (c == '\0') {
					line_data[i] = c;
					break;
				}
				line_data[i] = toupper(c);
			}
		}
		/* be sure the string is terminated if max size reached */
		if (i == MAX_LINE)
			line_data[i - 1] = '\0';

		/* send it back to the client on the data transfer socket */
		count = strlen(line_data) + 1; /* include '\0' in count */
		for (i = 0; i < count; i++) {
			c = line_data[i];
			rc = Socket_putc(c, connect_socket);
			if (rc == EOF) {
				printf("Socket_putc EOF or error\n");
				return; /* assume socket EOF ends service for this client */
			}
		}
	}	/* end while loop of the service process */

}
