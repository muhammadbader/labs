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
    
    int debug = 0;
    int status;
    if(argc>1)
    {
        for(int i=1;i<argc;i++)
        {
            if(strcmp(argv[i],"-d")==0)
            {
                debug=1;
                break;
            }
        }
    }
    // char* msgFromChild="hello";
    // int msgsize=strlen(msgFromChild);
    int p[2];
    if(pipe(p)<0)   //negative means the pipe failed 
        exit(1);
    if (debug) fprintf(stderr,"(parent_process > forking child 1)\n");
    int f = fork();
    
    if (f == 0)
    {
        close (fileno(stdout));// fclose(stdout)
        if (debug) fprintf(stderr,"(child1 > redirecting stdout to the write end of the pipe…)\n");
        int chldwrite = dup(p[1]);//duplicate the write end of the pipe
        close(p[1]);
        char* command[]={"ls","-l",NULL};
        if (debug) fprintf(stderr,"(child1 > going to execute cmd: ls)\n");
        execvp(command[0],command);
        // write(p[1],msgFromChild,msgsize);

    }
    else
    {
        if (debug) fprintf(stderr,"(parent_process > created process with id: %d)\n",f);
        if (debug) fprintf(stderr,"(parent_process > closing the write end of the pipe…)\n");
        close(p[1]);
        if (debug) fprintf(stderr,"(parent_process > forking child 1)\n");
        int f2 = fork();
        if(f2 == 0)
        {
            close (fileno(stdin));// fclose(stdin)
             if (debug) fprintf(stderr,"(child2 > redirecting stdout to the write end of the pipe…)\n");
            int chldwrite = dup(p[0]);//duplicate the read end of the pipe
            close(p[0]);
            if (debug) fprintf(stderr,"(child1 > going to execute cmd: tail)\n");
            char* command[]={"tail","-n","2",NULL};
            execvp(command[0],command);
        }else
        {
            if (debug) fprintf(stderr,"(parent_process > closing the read end of the pipe…)\n");
            close(p[0]);
            if (debug) fprintf(stderr,"(parent_process > waiting for child processes %d to terminate…)\n",f);
            waitpid(f,&status,0);
            if (debug) fprintf(stderr,"(parent_process > waiting for child processes %d to terminate…)\n",f2);
            waitpid(f2,&status,0);
             if (debug) fprintf(stderr,"(parent_process > exiting…)\n");
        }
        
        // printf("from parent\n");
        // char inmsg[msgsize];
        // read(p[0],inmsg,msgsize);
        // printf("%s\n",inmsg);
    }
    return 0;
}