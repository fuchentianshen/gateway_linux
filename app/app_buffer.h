#ifndef __APP_BUFFER_H__
#define __APP_BUFFER_H__

#include <stdlib.h>
#include <string.h>
#include "log.h"

// 小缓冲容器
typedef struct
{
    char *ptr;      // 数据指针
    int total_size; // 总大小
    int len;        // 当前已存储数据的长度
} SubBuffer;
// 缓冲区
typedef struct
{
    SubBuffer *sub_buffer[2];
    int read_index;
    int write_index;
} Buffer;

/**
 * @brief  初始化缓冲区
 *
 * @param total_size 内部缓冲区大小
 * @return Buffer* 缓冲区指针
 */
Buffer *app_buffer_init(int total_size);

/**
 * @brief  销毁缓冲区
 *
 * @param buffer 缓冲区指针
 */
void app_buffer_destroy(Buffer *buffer);

/**
 * @brief  向缓冲区写入数据
 *
 * @param buffer 缓冲区指针
 * @param data 数据指针
 * @param len 数据长度
 * @return int 0: 成功, -1: 失败
 */
int app_buffer_write(Buffer *buffer, char *data, int len);

/**
 * @brief  从缓冲区读取数据
 *
 * @param buffer 缓冲区指针
 * @param data_buff 存储接收到的数据的容器
 * @param buff_size 容器大小
 * @return int 读取到的数据长度, -1: 读取失败
 */
int app_buffer_read(Buffer *buffer, char *data_buff, int buff_size);
#endif /* __APP_BUFFER_H__ */
