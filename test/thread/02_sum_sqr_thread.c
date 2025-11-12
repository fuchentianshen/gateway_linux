#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

int cal_sqr(int num)
{
    int result = num * num;
    sleep(1);
    return result;
}
void *thread_fun(void *arg)
{
    int num = *(int *)arg;
    int *result = malloc(sizeof(int));
    *result = cal_sqr(num);
    // return result;
    pthread_exit(result);
}

int main(int argc, char const *argv[])
{
    int nums[] = {1, 2, 3, 4, 5};
    int sqrs[5];

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // 创建5个线程，每个线程计算一个数的平方
    pthread_t threads[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, thread_fun, (void *)&nums[i]);
    }
    // 等待所有线程完成
    for (int i = 0; i < 5; i++)
    {
        void *result;
        pthread_join(threads[i], &result);
        sqrs[i] = *(int *)result;
    }

    gettimeofday(&end, NULL);
    printf("time: %ld s\n", end.tv_sec - start.tv_sec);

    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += sqrs[i];
    }
    printf("sum: %d\n", sum);

    return 0;
}
