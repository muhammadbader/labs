#include "util.h"
#include <dirent.h>

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

typedef struct ent{
    int inode;
    int offset;
    short len;
    char buf[1];
    
    
} ent;

int main (int argc , char** argv)
{
    char buf[2000];
    /*ent* buffer;*/
    int fd;
    char* x=".";
    
    fd=system_call(SYS_OPEN,x,O_RDONLY,O_RDONLY);
    if(fd<0)
        system_call(EXIT,0,0,0);
    /*char *format="got file descriptor = ";
    char cfd[2];
    cfd[0]=*itoa(fd);
    cfd[1]='\n';
    system_call(SYS_WRITE,STDOUT,format,strlen(format));
    system_call(SYS_WRITE,STDOUT,cfd,2);*/

    int dataSize=system_call(DENTS,fd,buf,8192);/*using the get_dents system call*/ /*enter the char pointer here = buf*/
    /*datatSize has the number of bytes that were read */
    
    char *inode="Inode is ";
    char *offset=", offset is ";
    char *size=", size ";
    char* name=", name ";
    char* t=", type: ";
    int b=0;
    ent *tmp;
    
   while(b<dataSize)
    {
       
       tmp=(ent*)(buf+b);
       int i=0;
       while(tmp->buf[i]!='.'){ i++;}
       i++;
       
       if(argc==1||strcmp(&(tmp->buf)[i],&argv[1][2])==0)
       {
           char d_type=*(buf+b+tmp->len-1);
           char* type=(d_type == DT_REG) ?  "regular" :
            (d_type == DT_DIR) ?  "directory" :
            (d_type == DT_FIFO) ? "FIFO" :
            (d_type == DT_SOCK) ? "socket" :
            (d_type == DT_LNK) ?  "symlink" :
            (d_type == DT_BLK) ?  "block dev" :
            (d_type == DT_CHR) ?  "char dev" : "???";

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
            system_call(SYS_WRITE,STDOUT,param+3,1);
       }
       b=b+tmp->len;
    }

    /*closing the directory*/
    system_call(SYS_CLOSE,fd,0,0);
    return 0;
}