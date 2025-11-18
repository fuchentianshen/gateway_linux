#ifndef __APP_MQTT_H__
#define __APP_MQTT_H__

#include <stdint.h>
#include <string.h>
#include "MQTTClient.h"
#include "log.h"


#define ADDRESS     "tcp://192.168.42.38:1883"
#define CLIENTID    "b253ba38-daf6-4b37-984f-5d8fdc6a1cfc" 
#define TOPIC_GTC       "gateway_to_console"
#define TOPIC_CTG       "console_to_gateway"
#define QOS         1
#define TIMEOUT     10000L

/**
 * @brief 初始化MQTT模块
 * 
 * @return int8_t 0:成功  -1:失败
 */
int8_t app_mqtt_init(void);

/**
 * @brief 发布消息
 * 
 * @param json 消息数据
 * @return int8_t 0:成功  -1:失败
 */
int8_t app_mqtt_send(char *json);

/**
 * @brief 关闭MQTT模块
 * 
 * @return int8_t 
 */
int8_t app_mqtt_close(void);

/**
 * @brief 注册回调函数
 * 
 * @param callback 回调函数
 */
void app_mqtt_registerCallback(int8_t (*callback)(char *json));

#endif /* __APP_MQTT_H__ */
