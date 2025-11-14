#define _GNU_SOURCE
#include "app_pool.h"
#include <unistd.h>
int task_fun(void *arg)
{
    log_debug("子线程(%d)开始执行,参数:%s", gettid(), (char *)arg);
    return 0;
}
int main(int argc, char const *argv[])
{
    log_debug("主函数(%d)开始执行", gettid());
    // 初始化线程池
    app_pool_init(5);
    // 注册多个任务
    for (int i = 0; i < 10; i++)
    {
        app_pool_registerTask(task_fun, (void *)"hello world");
    }

    // 等待所有任务执行完毕
    sleep(1);

    return 0;
}
