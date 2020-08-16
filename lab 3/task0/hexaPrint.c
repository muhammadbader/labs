#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PrintHex(char* buffer,int length);
char* buffer="";
int main(int argc,char** argv){
    if(argc>1)
    {
        FILE* file=fopen(&argv[1][0],"rb");
        int length=0;
        while(!feof(file))
        {
            char* temp=buffer;
            length++;
            buffer=(char*)malloc(length*sizeof(char));
            int c[1];
            fread(c,1,1,file);
            strcpy(buffer,temp);
            buffer[length-1]=c[0];
        }
        PrintHex(buffer,length);
    }
    
    free(buffer);
    
}

void PrintHex(char* buffer,int length)
{
    for(int i=0;i<length;i++)
    {
        int mod;
        int ful;
        if(buffer[i]<0)
        {
            mod=((128+buffer[i])+128)%16;
            ful=((128+buffer[i])+128)/16;
        } 
        else
        {
            mod=buffer[i]%16;
            ful=buffer[i]/16;
        }
        
        
        if(ful>9)
        {
            ful-=10;
            printf("%c",'A'+ful);
        }
        else
        {
            printf("%c",'0'+ful);
        }

        if(mod>9)
        {
            mod-=10;
            printf("%c ",'A'+mod);
        }
        else
        {
            printf("%c ",'0'+mod);
        }
    }
}