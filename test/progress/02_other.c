#include <stdio.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    printf("other(%d)进程开始执行,argv[1] = %s\n", getpid(), argv[1]);
    return 0;
}
