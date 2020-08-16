#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0

void handler(int sig)
{
    printf("\nCaught Signal: %s\n", strsignal(sig));
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){ 

    signal(SIGINT,handler);
    signal(SIGCONT,handler);
    signal(SIGTSTP,handler);

	printf("Starting the program\n");

	while(1) {
        
		sleep(2);
	}

	return 0;
}