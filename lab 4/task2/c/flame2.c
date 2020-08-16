#include "util.h"

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_lseek 19
#define O_WRONLY 1
#define O_RDONLY 0
#define O_RDWR 2
#define SEEK_SET 0
#define LOWER_TO_UPPER 32
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define DENTS 141
#define EXIT 1

enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };


extern int system_call(int a, ...);
void debug_print(char* system_call_type, char* ret_val);
void printChar(char c);
void exitCall(int fd){system_call(SYS_CLOSE,fd,0,0);}
void getAddress(long int addr);

typedef struct ent{
    int inode;
    int offset;
    short len;
    char buf[1];  
} ent;

extern void infection();
extern infector(char *c);
int main (int argc , char** argv)
{
    
    char buf[2000];
    int fd;
    char* x=".";

    fd = system_call(SYS_OPEN,x,O_RDONLY,O_RDONLY);
    if(fd<0)
        system_call(EXIT,0,0,0);
    
    int debug,prefixOn=0,attachOn=0;
    char *prefix;
    int i;
    for(i=1;i<argc;i++)
    {
        if(!strncmp(argv[i],"-D",2))
        {/*active debug mode*/
            debug=1;
            char * de="debug mode active\n";
            system_call(SYS_WRITE,STDOUT,de,strlen(de));
            char* msg="the infection message";
            system_call(SYS_WRITE,STDOUT,msg,strlen(msg));
            char* intion="the infection address\n";
            system_call(SYS_WRITE,STDOUT,intion,strlen(intion));
            getAddress(infection);
            char* inftor="\nthe Infector address\n";
            system_call(SYS_WRITE,STDOUT,inftor,strlen(inftor));
            getAddress(infector);
        }
        else if(strncmp(argv[i],"-p",2)==0)
        {/*need to check about files without prefix*/
            prefixOn=1;
            prefix=&argv[i][2];
        }
        else if(strncmp(argv[i],"-a",2)==0)
        {
            attachOn=1;
            prefix=&argv[i][2];
        }
    }


    /*the File descriptor*/
    /*char *format="got file descriptor = ";
    char cfd[2];
    cfd[0]=*itoa(fd);
    cfd[1]='\n';
    system_call(SYS_WRITE,STDOUT,format,strlen(format));
    system_call(SYS_WRITE,STDOUT,cfd,2);*/

    int dataSize = system_call(DENTS,fd,buf,8192);/*using the get_dents system call*/ /*enter the char pointer here = buf*/
    /*datatSize has the number of bytes that were read */
    /*use the debug_print for debugging*/

    if(dataSize<=0)
    {
        exitCall(fd);
        return 0x55;
    }

    char *inode="Inode is ";
    char *offset=", offset is ";
    char *size=", size ";
    char* name=", name ";
    char* t=", type: ";
    int b=0;
    ent *tmp;
    
   while(b<dataSize) /* print the files names of some prefix*/
    {
       
       tmp=(ent*)(buf+b);
       /*int i=0;
       while(tmp->buf[i]!='.'){ i++;}
       i++;*/
       if(argc==1||strncmp(&tmp->buf,&argv[1][2],strlen(argv[1]-2))==0)
       {
            char d_type=*(buf+b+tmp->len-1);
            char* type=(d_type == DT_REG) ?  "regular" :
            (d_type == DT_DIR) ?  "directory" :
            (d_type == DT_FIFO) ? "FIFO" :
            (d_type == DT_SOCK) ? "socket" :
            (d_type == DT_LNK) ?  "symlink" :
            (d_type == DT_BLK) ?  "block dev" :
            (d_type == DT_CHR) ?  "char dev" : "???";
            
            int doit=0;
            /*don't touch those files*/
            /*if(strcmp(tmp->buf,"flame2") && strcmp(tmp->buf,"makefile") && strcmp(tmp->buf,".")&& strcmp(tmp->buf,".."))*/
            if(1)
            {
                
                if(prefixOn==1|| attachOn==1)
                {
                    if(strlen(prefix)<=strlen(tmp->buf))
                    {
                                                        
                                            
                                            /*int ans=strncmp(tmp->buf, prefix, strlen(prefix));
                                            system_call(SYS_WRITE,STDOUT,itoa(ans),1);
                                            system_call(SYS_WRITE,STDOUT,"\n",1);*/
                        if(strncmp(tmp->buf, prefix, strlen(prefix))==0)
                        {
                            doit = 1;
                        }
                    }
                }
            }
            else
            {
                doit=1;
            }

            if(doit)
            {
                int retuVal;
                if(debug)
                {
                                        
                    retuVal=system_call(SYS_WRITE,STDOUT,tmp->buf,strlen(tmp->buf));
                    system_call(SYS_WRITE,STDOUT,"\n",1);
                    debug_print(itoa(STDOUT),itoa(retuVal));
                    retuVal=system_call(SYS_WRITE,STDOUT,*itoa(tmp->len),strlen(itoa(tmp->len)));
                    debug_print(itoa(SYS_WRITE),itoa(retuVal));
                    system_call(SYS_WRITE,STDOUT,"\n",1);
                    retuVal=system_call(SYS_WRITE,STDOUT,type,strlen(type));
                    debug_print(itoa(SYS_WRITE),itoa(retuVal));
                    system_call(SYS_WRITE,STDOUT,"\n",1);
                }
                else
                {
                    system_call(SYS_WRITE,STDOUT,tmp->buf,strlen(tmp->buf));
                    system_call(SYS_WRITE,STDOUT,"\t",1);
                    system_call(SYS_WRITE,STDOUT,*itoa(tmp->len),strlen(itoa(tmp->len)));
                    system_call(SYS_WRITE,STDOUT,"\t",1);
                    system_call(SYS_WRITE,STDOUT,type,strlen(type));
                    system_call(SYS_WRITE,STDOUT,"\n",1);
                }

                if(attachOn) /*type==DT_REG ????*/
                {
                    infector(tmp->buf);
                    char* infect="We have Infected the file: ";
                    system_call(SYS_WRITE,STDOUT,infect,strlen(infect));
                    system_call(SYS_WRITE,STDOUT,tmp->buf,strlen(tmp->buf));
                    system_call(SYS_WRITE,STDOUT,"\n",1);
                }
            }
            /* skeletor for task 2b
            system_call(SYS_WRITE,STDOUT,inode,strlen(inode));
            char param[4];
            param[0]=*itoa(tmp->inode);
            param[1]=*itoa(tmp->offset);
            param[2]=*itoa(tmp->len);
            param[3]='\n';
            system_call(SYS_WRITE,STDOUT,param,1);
            system_call(SYS_WRITE,STDOUT,offset,strlen(offset));
            system_call(SYS_WRITE,STDOUT,param+1,1);
            system_call(SYS_WRITE,STDOUT,size,strlen(size));
            system_call(SYS_WRITE,STDOUT,param+2,1);
            system_call(SYS_WRITE,STDOUT,name,strlen(name));
            system_call(SYS_WRITE,STDOUT,tmp->buf,strlen(tmp->buf));
            system_call(SYS_WRITE,STDOUT,t,strlen(t));
            system_call(SYS_WRITE,STDOUT,type,strlen(type));
            system_call(SYS_WRITE,STDOUT,param+3,1);*/

       }
       b = b+tmp->len;
    }

    /*closing the directory*/
    exitCall(fd);
    return 0;
}
/*use if debug mode is activated*/
void debug_print(char* system_call_type, char* ret_val) {
	system_call(SYS_WRITE, STDERR, "ID of system call: ", 19);
	system_call(SYS_WRITE, STDERR, system_call_type, strlen(system_call_type));
	system_call(SYS_WRITE, STDERR, "\n", 1);

	system_call(SYS_WRITE, STDERR, "System call return value: ", 26);
	system_call(SYS_WRITE, STDERR, ret_val, strlen(ret_val));
	system_call(SYS_WRITE, STDERR, "\n", 1);
}

void printChar(char c) 
{
	char t[1];
	t[0] = c;
	system_call(SYS_WRITE, STDERR, t, 1);

}

/*print the address in hexa*/
void getAddress(long int addr) {

    long int remainder,quotient;
	int i=1,j,temp;
    quotient = addr;
	printChar('0');/*prefix for hexa*/
	printChar('x');/*we can use unstead => h*/

    while(quotient!=0) 
    {
    	temp = quotient % 16;
    	if(temp < 10) 
        {
            temp = temp + '0';
        }
    	else 
        {
            temp = temp + 'A';
        }
    	printChar(temp);
    	quotient /= 16;
    }
}