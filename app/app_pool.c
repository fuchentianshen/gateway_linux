#include "app_pool.h"

static mqd_t mq_fd;                   // 消息队列句柄
static char *mq_name = "/mq_gateway"; // 消息队列名称
static pthread_t *thread_pool;        // 线程池
static int thread_num = 0;            // 线程数量

// 线程函数
static void *thread_func(void *arg)
{
    Task task;
    while (1)
    {
        // 接收任务
        ssize_t len = mq_receive(mq_fd, (char *)&task, sizeof(Task), NULL);
        // 执行任务
        if (len == sizeof(Task))
        {
            task.task_fun(task.arg);
        }
    }
    return NULL;
}

int app_pool_init(int num_threads)
{
    // 创建消息队列
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Task);
    mq_fd = mq_open(mq_name, O_CREAT | O_RDWR, 0644, &attr);
    if (mq_fd == -1)
    {
        log_error("创建消息队列失败");
        return -1;
    }

    // 创建多个线程，并创建缓冲容器
    thread_num = num_threads;
    thread_pool = malloc(num_threads * sizeof(pthread_t));
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(thread_pool + i, NULL, thread_func, NULL);
    }
    return 0;
}

void app_pool_free()
{
    // 销毁消息队列
    mq_close(mq_fd);
    mq_unlink(mq_name);

    // 销毁线程池
    for (int i = 0; i < thread_num; i++)
    {
        pthread_cancel(thread_pool[i]);
        pthread_join(thread_pool[i], NULL);
    }
    free(thread_pool);
}

int app_pool_registerTask(int (*task_fun)(void *), void *arg)
{
    // 创建一个任务
    Task task = {
        .arg = arg,
        .task_fun = task_fun};

    // 发送任务到消息队列
    return mq_send(mq_fd, (char *)&task, sizeof(Task), 0);
}
