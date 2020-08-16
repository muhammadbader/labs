#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define EXIT_SUCCESS 0

void handler(int sig)
{
    // printf("\nLOOPER handling: %s\n", strsignal(sig));
    // pid_t x=wait(0);
    // int status;
    // int m=waitpid(x,&status,WNOHANG);
    // printf("%d",m);
    exit(EXIT_SUCCESS);
}

void suspending(int sig){
    pause();
}
void handleContinueSignal(int sig) {
    // myGlobalStaticContinueVariable = 1; // Or some other handling code
}

int main(int argc, char **argv){ 

    signal(SIGINT,handler);
    signal(SIGCONT,handleContinueSignal);/*modify*/
    signal(SIGTSTP,suspending);/*modify*/
    signal(SIGCHLD,handler);

	// printf("Starting the program\n");

    // int f=fork();
    // if(f==0) {
    //         sleep(2);
    //         return 0;
    // }else{
	while(1) {
        printf("1\n");
        sleep(1);	
    }
    // }
	return 0;
}