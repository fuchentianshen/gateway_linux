#include "daemon_runner.h"

static SubProcess *sub_process_list[SUB_PROCESS_COUNT];
static char *cmd_params[SUB_PROCESS_COUNT] = {"app", "ota"};

static int is_running = 1;

static void exit_handler(int signal)
{
    is_running = 0;
}
int daemon_runner_run()
{
    // 注册退出信号处理
    signal(SIGINT, exit_handler);
    signal(SIGTERM, exit_handler);

    // 初始化所有子进程
    for (int i = 0; i < SUB_PROCESS_COUNT; i++)
    {
        sub_process_list[i] = daemon_sub_process_init(cmd_params[i]);
    }

    // 不断的检查启动被守护的子进程
    while (is_running)
    {
        for (int i = 0; i < SUB_PROCESS_COUNT; i++)
        {
            daemon_sub_process_checkStart(sub_process_list[i]);
        }
        sleep(1);
    }

    // 停止所有子进程
    for (int i = 0; i < SUB_PROCESS_COUNT; i++)
    {
        daemon_sub_process_stop(sub_process_list[i]);
    }
    return 0;
}
