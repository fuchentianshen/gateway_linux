#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *first_thread_func(void *arg)
{
    printf("第一个分线程(%d)开始执行\n",gettid());
    return NULL;
}

void *second_thread_func(void *arg)
{
    printf("第二个分线程(%d)开始执行\n",gettid());
    return NULL;
}
int main(int argc, char const *argv[])
{
    printf("main线程(%d)开始执行\n",gettid());

    //创建两个分线程
    pthread_t pt1,pt2;//线程标识
    pthread_create(&pt1,NULL,first_thread_func,NULL);
    pthread_create(&pt2,NULL,second_thread_func,NULL);

    printf("main线程(%d)继续执行,pt1=%ld,pt2=%ld\n",gettid(),pt1,pt2);

    pthread_join(pt1,NULL);
    pthread_join(pt2,NULL);

    printf("main线程(%d)结束执行\n",gettid());//主线程结束后,还未执行的分线程不会执行
    return 0;
}

