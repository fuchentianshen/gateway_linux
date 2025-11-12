#include "app_mqtt.h"
#include "unistd.h"

int8_t msg_callback(char *json)
{
    log_debug("接收到的json: %s", json);
    return 0;
}
int main(int argc, char const *argv[])
{
    // 初始化mqtt
    app_mqtt_init();
    // 注册回调函数
    app_mqtt_registerCallback(msg_callback);
    // 发送消息
    app_mqtt_send("{\"device_id\":1,\"cur_angle\":3600,\"motor_status\":\"on\"}");

    // 休眠100s
    sleep(100);
    return 0;
}
