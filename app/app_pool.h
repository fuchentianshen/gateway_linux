#ifndef __APP_POOL_H__
#define __APP_POOL_H__

#include <mqueue.h>
#include <pthread.h>
#include "log.h"
#include <stdlib.h>

typedef struct
{
    int (*task_fun)(void *); // 任务函数
    void *arg;               // 任务函数的参数
} Task;

/**
 * @brief 初始化线程池模块
 *
 * @param num_threads 线程数
 * @return int 0：成功 -1：失败
 */
int app_pool_init(int num_threads);

/**
 * @brief 释放线程池模块
 *
 */
void app_pool_free();

/**
 * @brief 注册任务
 *
 * @param task_fun 任务函数
 * @param arg 任务参数
 * @return int
 */
int app_pool_registerTask(int (*task_fun)(void *), void *arg);

#endif /* __APP_POOL_H__ */
