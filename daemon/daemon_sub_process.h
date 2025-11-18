#ifndef __DAEMON_SUB_PROCESS_H__
#define __DAEMON_SUB_PROCESS_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <signal.h>
#include "log.h"

#define MAX_FAIL_COUNT 5

// 子进程结构体
typedef struct
{
    pid_t pid;       // 子进程ID
    char *cmd_param; // 子进程启动参数
    int fail_count;  // 子进程失败次数
} SubProcess;

/**
 * @brief 子进程初始化
 * 
 * @param cmd_param 子进程启动参数
 * @return SubProcess* 子进程结构体指针
 */
SubProcess *daemon_sub_process_init(char *cmd_param);

/**
 * @brief 子进程检查启动
 * 
 * @param sub_process 子进程结构体指针
 * @return int 0:启动成功 -1:启动失败
 */
int daemon_sub_process_checkStart(SubProcess *sub_process);

/**
 * @brief 子进程停止
 * 
 * @param sub_process 
 */
void daemon_sub_process_stop(SubProcess *sub_process);

#endif /* __DAEMON_SUB_PROCESS_H__ */
