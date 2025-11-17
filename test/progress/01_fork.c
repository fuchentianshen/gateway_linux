#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
int main(int argc, char const *argv[])
{
    printf("main进程(%d)开始执行\n", getpid());
    int value = 100;

    pid_t pid = fork();

    if (pid == -1)
    {
        printf("创建子进程失败\n");
    }
    else if (pid == 0)
    {
        value++;
        printf("子进程(%d)开始执行,父进程=%d\n", getpid(), getppid());
        printf("value=%d,value的地址:%p\n",value, &value);
    }
    else
    {
        value--;
        printf("父进程(%d)开始执行,子进程=%d\n", getpid(), pid);
        printf("value=%d,value的地址:%p\n",value, &value);
    }

    printf("进程(%d)执行完毕\n", getpid());

    return 0;
}
