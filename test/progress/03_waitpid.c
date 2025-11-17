#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char const *argv[])
{
    printf("main进程(%d)开始执行\n", getpid());

    pid_t pid = fork();

    if (pid == -1)
    {
        printf("创建子进程失败\n");
    }
    else if (pid == 0)
    {
        printf("子进程(%d)开始执行,父进程=%d\n", getpid(), getppid());
        char *path="/bin/ping";
        char *argv[]={path,"www.baidu.com","-c","5",NULL};
        execve(path,argv,NULL);
        printf("未找到指定命令,execve函数执行失败");
    }
    else
    {
        printf("父进程(%d)开始执行,子进程=%d\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        printf("子进程(%d)执行完毕,退出状态码=%d\n", pid, status);
    }

    printf("进程(%d)执行完毕\n", getpid());

    return 0;
}
