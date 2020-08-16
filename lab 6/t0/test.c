#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char** argv)
{
    char* cat="cat";
    char *ry="muhammad";
    execvp(cat,ry);
    return 0;
}
