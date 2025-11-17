#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static int is_running = 1;

// 使用资源
void useResource()
{
    printf("使用资源\n");
}

// 释放资源
void freeResource()
{
    printf("释放资源\n");
}

void exit_handler(int signal)
{
    printf("收到信号%d\n", signal);
    if (signal == SIGINT)
    {
        freeResource();
        exit(EXIT_SUCCESS);
    }
    else
    {
        is_running = 0;
    }
}
int main(int argc, char const *argv[])
{
    signal(SIGINT, exit_handler);  // Ctrl+C
    signal(SIGTERM, exit_handler); // kill
    useResource();

    while (is_running)
    {
        printf("程序运行中...\n");
        sleep(1);
    }

    freeResource();
    return 0;
}
