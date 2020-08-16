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

extern int system_call(int a, ...);
printoutputFilePath(int fkind,int ret);
printinputFilePath(int fkind,int ret);
printD(int fkind,int ret);

int main (int argc , char** argv){
	int debugt=0;
	int i;
	int out=STDOUT;
	int in=STDIN; 
	int y;
	
	for( i = 1; i < argc; i++){
		if(strcmp(argv[i], "-D") == 0){ /* debug mode */
			debugt = 1;
		}
		else if(strncmp(argv[i], "-o",2) == 0){ /* debug mode */
			char filename[255];
			for (y = 0; y <strlen(argv[i]); y++)
			{
			   filename[y]=argv[i][y + 2];
			}
			out=system_call(SYS_OPEN, filename, 0x101, 0644);
			if(debugt)			
				printD(SYS_OPEN,in);
			if(out < 0)
          			return 1;  /* error */
		}
		else if(strncmp(argv[i], "-i",2) == 0){ /* debug mode */
			char filename[255];
			for ( y = 0; y <strlen(argv[i]); y++)
			{
			   filename[y]=argv[i][y + 2];
			}
			in=system_call(SYS_OPEN, filename, O_RDONLY, 0644);
			if(debugt)			
				printD(SYS_OPEN,in);
			if(in < 0)
          		  return 1;  /* error */
			
		}
	}
	char str[128];
	int ret=system_call(SYS_READ, in, str, 255);	
	if(debugt)
	{
		printD(SYS_READ,ret);
		printD(SYS_WRITE,in);
		printD(SYS_WRITE,out);
		for( i=0;i<strlen(str);i++){
		if(str[i] >= 'a' && str[i] <= 'z') /*lowercase*/
		str[i] =str[i]- LOWER_TO_UPPER; /*convert to uppercase*/
		}
		ret=system_call(SYS_WRITE, out, str,strlen(str));
		printD(SYS_WRITE,ret);
		printinputFilePath(SYS_WRITE,in);
		printoutputFilePath(SYS_WRITE,out);
	}else{
		for( i=0;i<strlen(str);i++){
		if(str[i] >= 'a' && str[i] <= 'z') /*lowercase*/
			str[i] =str[i]- LOWER_TO_UPPER; /*convert to uppercase*/
	}
		ret=system_call(SYS_WRITE, out, str,strlen(str));
	}

	
	
	return 0;
}

printinputFilePath(int fkind,int ret)
{
	if(ret==0)
	{
		char* file="input file path = stdin\n";
		system_call(SYS_WRITE,STDERR,file,strlen(file));
	}
	else
	{
		char* file="input file path = ";
		system_call(SYS_WRITE,STDERR,file,strlen(file));
		char f[2];
		f[0]=*itoa(ret);
		f[1]='\n';
		system_call(SYS_WRITE,STDERR,f,2);
	}
}

printoutputFilePath(int fkind,int ret)
{
	if(ret==1)
	{
		char* file="output file path = stdout\n";
		system_call(SYS_WRITE,STDERR,file,strlen(file));
	}
	else
	{
		
		char* file="output file path = ";
		system_call(SYS_WRITE,STDERR,file,strlen(file));
		char f[2];
		f[0]=*itoa(ret);
		f[1]='\n';
		system_call(SYS_WRITE,STDERR,f,2);
	}
	
}

printD(int fkind,int ret){
	char dPrint[4];
	dPrint[0]=*itoa(fkind);
	dPrint[1]='\t';
	dPrint[2]=*itoa(ret);
	dPrint[3]='\n';
	
	system_call(SYS_WRITE, STDERR, dPrint, 4);

}