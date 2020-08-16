#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <fcntl.h>

int main(int argc, char** argv)
{
    char *x=getenv("HOME");
    printf("%s\n",x);
    return 0;
}
