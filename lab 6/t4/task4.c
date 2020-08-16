#include "LineParser.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>   /* #define PATH_MAX 4096 */

#define MAXIN 2048

cmdLine *command;

void changeDir() { /* a shell feature "changeDir" that allows the "user" to change the current working directory */
  char dri[100];

  if(childf(command->arguments[1]) == -1)
    fprintf(stderr, "changeDir failed\n");
  else /* printing new working directory */
	  printf("The new working directory: %s\n", getcwd(dri, 100));
} /* childf(".."); - parent dir */

void execute(pid_t chipidd, int d_flag) {
  /* Parent proccesse */
  if(d_flag && chipidd)
    fprintf(stderr, "Child PID: %d\n", chipidd); /* process ID of the newly-born child */

  /* Child proccesses */
  if(chipidd == 0) {
    execvp(command->arguments[0], command->arguments);
    perror(command->arguments[0]);
    _exit(EXIT_FAILURE);
  }
  /* Parent proccesse */
  else { 
    if(command->blocking == 1) {
      int status;
      waitpid(chipidd, &status, 0);
    }
  }
}


int main(int argc, char **argv,char **envp) {
  int d_flag = 0;
  char buffer[PATH_MAX];
  char* CurrWorkingDire;
  char inputLine[MAXIN];
  int pipefd[2];

  if(argc > 1) {
	  if(strcmp("-d", argv[1]) == 0) 
	    d_flag = 1; 
	}
	
  while(1) {
    CurrWorkingDire = getcwd(buffer, PATH_MAX);
    if(CurrWorkingDire != NULL)
	    printf("The current working directory: %s\n", buffer);
    else
      perror("Failure getting the work directory\n");

    fgets(inputLine, MAXIN, stdin); /* Read a line from the "user" */
	
	  if(d_flag)
		  fprintf(stderr, "Parent PID: %d\nExecuting command: %s", getpid(), inputLine);
    
    if(strcmp(inputLine, "quit\n") == 0) /* exit the shell "normally" */
		  break;
    
    if(strlen(inputLine) == 1) /* inputLine = '\n' (new line) */
	    continue;

    command = parseCmdLines(inputLine);

    if(command->next != NULL) { /* Here the shell run commands like: ls|wc -l which basically counts the number 
                               of files/directories under the current working dir. */
      if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
      }
            
      pid_t pid1, pid2;
            
      switch(pid1 = fork()){
        default: /* Parent proccesse */
          close(pipefd[1]);
          break;
        case 0: /* Child proccesse */
          fclose(stdout);
          dup(pipefd[1]);
          close(pipefd[1]);
          execute(pid1, d_flag);
          break;
        case -1:
          perror("fork");
          exit(EXIT_FAILURE);       
      }
            
      switch(pid2 = fork()){
        default: /* Parent proccesse */
          close(pipefd[0]);
          break;
        case 0: /* Child proccesse */
          fclose(stdin);
          dup(pipefd[0]);
          close(pipefd[0]);
          command = command->next;
          execute(pid2, d_flag);
          break;
        case -1:
          perror("fork");
          exit(EXIT_FAILURE);       
      }
    
      waitpid(pid1, NULL, 0);
      waitpid(pid2, NULL, 0);
    }

    else if(strcmp(command->arguments[0], "changeDir") == 0)
      changeDir();
    
    else {
      pid_t pid;
      switch(pid = fork()) {
        default: /* Parent and child proccesses */
          execute(pid, d_flag);
          break; 
        case -1:
          perror("fork");
          freeCmdLines(command);
          exit(EXIT_FAILURE);
      }
    }

    freeCmdLines(command);
  }
  return EXIT_SUCCESS;
}