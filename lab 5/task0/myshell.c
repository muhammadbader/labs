#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "LineParser.h"

void printCommand(cmdLine* c);

execute(cmdLine *pCmdLine);


int main() {
   char cwd[PATH_MAX];
   char input[2048];
   cmdLine* command;
   while(1)
   {
       if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s $ ", cwd);
            fgets(input,2048,stdin);
            if(!strncmp(input,"quit",4)) break;
            command=parseCmdLines(input);
            // execute(command);
            printCommand(command);
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
    for(int i=0;i<strlen(c->arguments);i++){
        printf("%s\n",c->arguments[i]);
    }
    printf("inputRedirect: %s\n",c->inputRedirect);
    printf("outputRedirect: %s\n",c->outputRedirect);
    printf("index: %d\n",c->idx);
    printf("blocking: %c\n",c->blocking);
    printf("\nfinish\n");
    if(c->next!=NULL) printCommand(c->next);
}

execute(cmdLine *pCmdLine)
{
    // char *path="./test";
    // char* args[]={"/task0/ls",NULL};
    int f = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
    printf("%d\n",f);
    if(f == -1){
        perror("EXECUTION failed\n");
        exit(0x55);
    }
}		