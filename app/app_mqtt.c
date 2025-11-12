#include "app_mqtt.h"

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
static int8_t (*msg_callback)(char *json) = NULL;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log_debug("发送消息成功");
}

// 收到信息的回调函数
// 1：接收消息成功 0：接收消息失败
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int result = 0;
    if (msg_callback != NULL)
    {
        result = msg_callback(message->payload) == 0 ? 1 : 0;
    }

    // 释放内存
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return result;
}

void connlost(void *context, char *cause)
{
    log_error("mqtt客户端断开连接，原因：%s", cause);
}

int8_t app_mqtt_init(void)
{
    // 创建mqtt客户端
    if (MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt客户端创建失败");
        return -1;
    }
    // 设置回调函数
    if (MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt客户端注册回调函数失败");
        MQTTClient_destroy(&client);
        return -1;
    }
    // 连接mqtt服务器
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt客户端连接失败");
        MQTTClient_destroy(&client);
        return -1;
    }
    // 订阅主题
    if (MQTTClient_subscribe(client, TOPIC_CTG, QOS) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt客户端订阅主题失败");
        MQTTClient_disconnect(client, TIMEOUT);
        MQTTClient_destroy(&client);
        return -1;
    }
    log_debug("mqtt客户端初始化成功");

    return 0;
}

int8_t app_mqtt_send(char *json)
{
    pubmsg.payload = json;
    pubmsg.payloadlen = strlen(json);
    pubmsg.qos = QOS;

    if (MQTTClient_publishMessage(client, TOPIC_GTC, &pubmsg, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt客户端发送消息失败");
        return -1;
    }

    return 0;
}

int8_t app_mqtt_close(void)
{
    MQTTClient_unsubscribe(client, TOPIC_CTG);
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
    log_debug("mqtt客户端关闭成功");
    return 0;
}

void app_mqtt_registerCallback(int8_t (*callback)(char *json))
{
    msg_callback = callback;
}
