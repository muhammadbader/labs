#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/*  pipe() use
        int pipe(int fds[2]);
        Parameters :
        fd[0] will be the fd(file descriptor) for the 
                read end of pipe.
        fd[1] will be the fd for the write end of pipe.
                Returns : 0 on Success.
        -1 on error.
*/
int main(int argc,char** argv)
{
    char* msgFromChild = "hello";
    int msgsize = strlen(msgFromChild);
    int p[2];
    if(pipe(p)<0)   //negative means the pipe failed 
        exit(1);
    int f = fork();
    if (f==0)
    {
        write(p[1],msgFromChild,msgsize);
    }
    else
    {
        // printf("from parent\n");
        char inmsg[msgsize];
        read(p[0],inmsg,msgsize);
        printf("%s\n",inmsg);
    }
    return 0;
}