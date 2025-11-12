#include "app_message.h"
#include "stdlib.h"

int main(int argc, char const *argv[])
{
    uint8_t bytes[] = {0x01, 0x00, 0x00, 0x61, 0x45, 0x01, 0x00};
    char *json = app_message_bytesToJson(bytes, 7);
    log_debug("生成json: %s", json);
    free(json);

    char *down_json = "{\"device_id\": 1, \"target_angle\": 3600, \"max_speed\": 60}";
    uint8_t bytes_buf[100];
    uint8_t len = app_message_jsonToBytes(down_json, bytes_buf, 100);

    printf("len:%d\r\n", len);
    for (uint8_t i = 0; i < len; i++)
    {
        printf("%02x ", bytes_buf[i]);
    }
    printf("\r\n");
    
    return 0;
}
