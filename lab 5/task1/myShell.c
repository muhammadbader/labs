#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void printCommand(cmdLine* c);

void execute(cmdLine *pCmdLine);
void changeDirectory(char* dir);
#define max_INPUT 2048
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


int main() {
   char cwd[PATH_MAX];
   char input[2048];
   cmdLine* command;
   while(1)
   {
       if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s$ ", cwd);
            fgets(input,max_INPUT,stdin);
            if(!strncmp(input,"quit",4)) break;
            if(strlen(input)==1) continue;/* new line */
            command=parseCmdLines(input);
            execute(command);
            // printCommand(command);
            freeCmdLines(command);
        } else {
            perror("getcwd() error");
            return 1;
        }
   }
   return 0;
}

void printCommand(cmdLine* c)
{
    printf("argCount: %d\n",c->argCount);
    printf("arguments: \n");
    for(int i=0;i<c->argCount;i++){
        printf("%s\n",c->arguments[i]);
    }
    printf("inputRedirect: %s\n",c->inputRedirect);
    printf("outputRedirect: %s\n",c->outputRedirect);
    printf("index: %d\n",c->idx);
    printf("blocking: %c\n",c->blocking);
    if(c->next!=NULL) printCommand(c->next);
}

void execute(cmdLine *pCmdLine)
{
    int debug=0,cd=0;
    char* dir;
    for(int i=0;i<pCmdLine->argCount;i++){
        if(strncmp(pCmdLine->arguments[i],"-d",2)==0) 
        {
            debug=1;
        }else if(strncmp(pCmdLine->arguments[i],"cd",2)==0) 
        {
            cd=1;
            dir=pCmdLine->arguments[i+1];
        }
    }
    if(cd==1)
    {
         changeDirectory(dir);
         if(pCmdLine->next!=NULL) execute(pCmdLine->next);
         return;
    }
    int f;
    // char* args[]={"/bin/ls",NULL};
    if(fork()==0)
    {
        if(debug)
        {
            fprintf(stderr,"Child => PPID: %d PID: %d\n", getppid(), getpid());
            fprintf(stderr,"executing Commands:\n");
            for(int i=0;i<pCmdLine->argCount;i++)
            {
                fprintf(stderr,"%s\n",pCmdLine->arguments[i]);
            }
        }
        f = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        if(f==-1){
            perror(pCmdLine->arguments[0]);
            _exit(EXIT_FAILURE);
        }
            
    }
    else
    {
        if(debug){
            fprintf(stderr,"Parent => PID: %d\n", getpid());
            // fprintf(stderr,"Waiting for child process to finish.\n");
            wait(NULL);
            // fprintf(stderr,"Child process finished.\n");
        }else{
            if(pCmdLine->blocking==1){
            int status;
            waitpid(f, &status, 0);
            }
        }
         
    }
    if(f == -1){
        perror("EXECUTION failed\n");
        _exit(1);
    }
    if(pCmdLine->next!=NULL) execute(pCmdLine->next);
}

void changeDirectory(char* dir)
{
    if(chdir(dir)==-1)
    {
        fprintf(stderr, "cd failed\n");
    }else
    {
        char cwd[PATH_MAX];
        fprintf(stderr, "we got a new directory: %s\n",getcwd(cwd, sizeof(cwd)));
    }
    
}