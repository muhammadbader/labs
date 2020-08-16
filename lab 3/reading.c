#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct fun_desc {
  char *name;
  void (*fun)();
};

void printx(){
    printf("printx\n");
}

void printy(int x){
    printf("printy %d\n",x);
}

int main()
{
    struct fun_desc x[]={{"x",printx},{"y",printy}};
    x[0].fun();
    x[1].fun(3);
}