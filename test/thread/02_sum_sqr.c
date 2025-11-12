#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

int cal_sqr(int num)
{
    int result = num * num;
    sleep(1);
    return result;
}

int main(int argc, char const *argv[])
{
    int nums[] = {1, 2, 3, 4, 5};
    int sqrs[5];

    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 5; i++)
    {
        sqrs[i] = cal_sqr(nums[i]);
    }
    gettimeofday(&end, NULL);
    printf("time: %ld s\n",end.tv_sec - start.tv_sec);

    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += sqrs[i];
    }
    printf("sum: %d\n", sum);

    return 0;
}
