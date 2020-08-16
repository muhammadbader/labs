#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* toHex(unsigned int dec)
{
    
    char *ans=calloc(2,sizeof (char));
    unsigned short fac=1;
    int counter=1;
    while(counter>=0)
    {
        int x=(dec%16);
        if(x>=0&&x<=9)
        {
            ans[counter]=x+'0';
        }
        else
        {
            ans[counter]=x-10+'A';
        }
        counter--;
        
        fac=fac*16;
        dec=dec/16;
    }
    
    return ans;
}

int main()
{
    char* c="check芭MyHex";//63 68 65 63 6B | AA DD 4D | 79 0C 48 65 78
    char d='k';//6B
    
   
   for(int i=0;i<strlen(c);i++)
   {
       printf("%s ",toHex(c[i]));
   }
   printf("\n");

   
}