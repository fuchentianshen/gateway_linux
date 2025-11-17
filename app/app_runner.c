#include "app_runner.h"

static int is_running = 1;
void exit_handler(int signal)
{
    is_running = 0;
}

void app_runner_run(void)
{
    // 注册结束信号处理函数
    signal(SIGINT, exit_handler);  // 键盘输入Ctrl+C
    signal(SIGTERM, exit_handler); // kill -15

    // 初始化设备
    app_device_init(DEVICE_FILE);

    // 启动设备
    app_device_start();

    while (is_running)
    {
        sleep(1);
    }

    //释放设备
    app_device_free();
}
