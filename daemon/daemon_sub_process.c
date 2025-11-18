#include "daemon_sub_process.h"

SubProcess *daemon_sub_process_init(char *cmd_param)
{
    SubProcess *sub_process = (SubProcess *)malloc(sizeof(SubProcess));
    sub_process->cmd_param = cmd_param;
    sub_process->fail_count = 0;
    sub_process->pid = -1;
    return sub_process;
}

int daemon_sub_process_checkStart(SubProcess *sub_process)
{
    // 检查进程是否在运行，如果在运行正常结束
    int status = 0;
    if (sub_process->pid > 0 && waitpid(sub_process->pid, &status, WNOHANG) == 0)
    {
        return 0;
    }

    // 如果子进程以失败结束，则重启
    if (status != 0)
    {
        sub_process->fail_count++;
        if (sub_process->fail_count > MAX_FAIL_COUNT)
        {
            // 重启
            reboot(RB_AUTOBOOT);
        }
    }

    // 子进程没有运行：没有运行过或者运行正常结束
    sub_process->pid = fork();
    if (sub_process->pid == 0)
    {
        // 子进程
        char *argv[] = {EXE_PATH, sub_process->cmd_param, NULL};
        log_debug("在子进程(%d)启动：%s", getpid(), sub_process->cmd_param);
        execve(EXE_PATH, argv, NULL);
        // execve失败
        _exit(EXIT_FAILURE);
    }

    return 0;
}

void daemon_sub_process_stop(SubProcess *sub_process)
{
    // kill子进程，发送信号
    kill(sub_process->pid, SIGTERM);
    // 等待子进程结束
    waitpid(sub_process->pid, NULL, 0);
    // 释放内存
    free(sub_process);
}
