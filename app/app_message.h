#ifndef __APP_MESSAGE_H__
#define __APP_MESSAGE_H__

#include "cJSON.h"
#include "log.h"
#include "stdint.h"
#include "string.h"

/**
 * @brief 字符数组转为JSON
 * 
 * @param bytes 字符数组
 * @param len 数组长度
 * @return char* json字符串
 */
char *app_message_bytesToJson(uint8_t *bytes,uint8_t len);

/**
 * @brief json字符串转为字符数组
 * 
 * @param json json字符串
 * @param bytes_buf 存储数据的容器
 * @param buf_size 容器的大小
 * @return uint8_t 实际存储的数据长度
 */
uint8_t app_message_jsonToBytes(char *json,uint8_t *bytes_buf,uint8_t buf_size);

#endif /* __APP_MESSAGE_H__ */
