#include <stdio.h>

int main(int argc, char** argv)
{
    int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3];
    printf("i array\n");
    printf("%p\n%p\n",iarray,iarray+1);
     printf("f array\n");
    printf("%p\n%p\n",farray,farray+1);
     printf("d array\n");
    printf("%p\n%p\n",darray,darray+1);
     printf("c array\n");
    printf("%p\n%p\n",carray,carray+1);
    
}
