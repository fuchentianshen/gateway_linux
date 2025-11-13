#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>

static mqd_t mqd;

void *first_thread_func(void *arg)
{
    sleep(1);
    printf("第一个分线程开始执行,开始接收消息\n");
    // 接收消息
    char buf[100];
    ssize_t len = mq_receive(mqd, buf, 100, NULL);
    if (len == -1)
    {
        printf("接收消息失败\n");
    }
    else
    {
        printf("接收消息成功,消息内容为:%.*s\n", (int)len, buf);
    }

    return NULL;
}

void *second_thread_func(void *arg)
{
    printf("第二个分线程开始执行,开始发送消息\n");
    // 发送消息
    mq_send(mqd, "hello world", 11, 1);
    printf("发送消息成功\n");

    return NULL;
}
int main(int argc, char const *argv[])
{
    // 创建消息队列
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    mqd = mq_open("/mq_test", O_RDWR | O_CREAT, 0644, &attr);
    if (mqd == -1)
    {
        printf("创建消息队列失败\n");
        return -1;
    }

    // 创建两个分线程
    pthread_t pt1, pt2; // 线程标识
    pthread_create(&pt1, NULL, first_thread_func, NULL);
    pthread_create(&pt2, NULL, second_thread_func, NULL);

    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);

    return 0;
}
