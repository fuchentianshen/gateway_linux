#include "app_message.h"

/*
字节数组消息：01 00 00 61 45 01 00
json消息：{"device_id": 1, "cur_angle": 3600, "motor_status": "on"}
*/
char *app_message_bytesToJson(uint8_t *bytes, uint8_t len)
{
    if (len < 7)
    {
        log_error("数组长度不够");
        return NULL;
    }

    // 创建json对象
    cJSON *json_obj = cJSON_CreateObject();
    // 从字符数组中提取数据
    uint8_t device_id = bytes[0];
    float cur_angle;
    memcpy(&cur_angle, bytes + 1, 4);
    char *motor_status = bytes[5] == 0 ? "off" : "on";

    // 添加数据到json
    cJSON_AddNumberToObject(json_obj, "device_id", device_id);
    cJSON_AddNumberToObject(json_obj, "cur_angle", cur_angle);
    cJSON_AddStringToObject(json_obj, "motor_status", motor_status);

    // 转换为json字符串
    char *json_str = cJSON_PrintUnformatted(json_obj);

    cJSON_Delete(json_obj);
    return json_str;
}

/*
json消息：{"device_id": 1, "target_angle": 3600, "max_speed": 60}
字节数组消息：01 00 00 61 45 00 00 70 42
*/
uint8_t app_message_jsonToBytes(char *json, uint8_t *bytes_buf, uint8_t buf_size)
{
    // 计算字节数组长度
    uint8_t len = 1 + 4 + 4;
    if (len > buf_size)
    {
        log_error("缓冲区大小不够");
        return 0;
    }
    //解析json字符串
    cJSON *json_obj = cJSON_Parse(json);

    // 从json中获取数据
    uint8_t device_id = cJSON_GetObjectItem(json_obj, "device_id")->valueint;
    float target_angle = cJSON_GetObjectItem(json_obj, "target_angle")->valueint;
    float max_speed = cJSON_GetObjectItem(json_obj, "max_speed")->valueint;

    // 将数据写入字节数组
    bytes_buf[0] = device_id;
    memcpy(bytes_buf + 1, &target_angle, 4);
    memcpy(bytes_buf + 5, &max_speed, 4);

    cJSON_Delete(json_obj);
    return len;
}
