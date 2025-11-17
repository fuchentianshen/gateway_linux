#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__

#include "app_modbus.h"
#include "app_buffer.h"
#include "app_message.h"
#include "app_mqtt.h"
#include "app_pool.h"
#include "log.h"
#include <string.h>

#define POOL_SIZE 5
#define BUFFER_SIZE 1024
#define DEVICE_SIZE 50
/**
 * @brief 设备初始化
 * 
 * @param filename 设备配置文件
 * @return int 0:成功, -1:失败
 */
int app_device_init(char *filename);

/**
 * @brief 启动设备
 * 
 */
void app_device_start(void);

/**
 * @brief 释放设备
 * 
 */
void app_device_free(void);

#endif /* __APP_DEVICE_H__ */
