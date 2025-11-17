#include "app_device.h"

static Buffer *up_buffer;           // 上行缓冲区
static Buffer *down_buffer;         // 下行缓冲区
static int device_ids[DEVICE_SIZE]; // 设备id列表
static int device_num = 0;          // 当前设备数量
static pthread_t read_thread;       // 读取设备数据的线程标识
int app_device_init(char *filename)
{
    app_mqtt_init();
    app_pool_init(POOL_SIZE);
    up_buffer = app_buffer_init(BUFFER_SIZE);
    down_buffer = app_buffer_init(BUFFER_SIZE);
    app_modbus_init(filename);
    return 0;
}

/*
1. 下行操作流程
   1. 注册回调函数接收上游设备发送的数据，app_mqtt_registerCallback(receive_msg_callback)
      1. 将接收到的json数据转换为字节数组数据，app_message_jsonToBytes(json)
      2. 将字节数组数据存储到下行缓冲区，app_buffer_write(down_buffer)
      3. 通过线程池从下行缓冲区取出数据发送给下游设备，app_pool_registerTask(write_task_func)
   2. write_task_func
      1. 将缓冲区的数据取出，app_buffer_read()
      2. 读出device_id将字节数组数据转换为保持寄存器数据
      3. 将数据写入到保持寄存器，app_modbus_write()
      4. 将device_id 加入到设备id列表
2. 上行操作流程
   1. 创建一个线程不断得轮询下游设备的消息，read_thread_func
      1. 从输入寄存器读取数据，app_modbus_read
      2. 将读取到的数据转换为字节数组数据，存储到上行缓冲区，app_buffer_send(up_buffer)
      3. 将后续流程交给线程池处理，app_pool_registerCallback(send_task_func)
   2. send_task_func
      1. 从上行缓冲区读取数据，app_buffer_read
      2. 将读取到的数据转换为json数据，app_message_jsonToBytes()
      3. 通过mqtt将json数据发送到上游设备
      4. 如果设备已停止运行，从设备id列表删除
*/

// 将device_id加入到设备id列表
void add_device_id(int device_id)
{
    // 遍历设备id列表，如果已存在则不需要添加
    for (int i = 0; i < device_num; i++)
    {
        if (device_ids[i] == device_id)
        {
            return;
        }
    }

    // 将device_id加入到设备id列表
    device_ids[device_num++] = device_id;

    // 显示设备id列表
    printf("device_ids: ");
    for (int i = 0; i < device_num; i++)
    {
        printf("%d ", device_ids[i]);
    }
    printf("\n");
}

// 将数据写给下游设备
int write_task_func(void *arg)
{
    // 从下行缓冲区读取数据
    char bytes_buff[128];
    int len = app_buffer_read(down_buffer, bytes_buff, sizeof(bytes_buff));

    // 将字节数组数据转换为保持寄存器数据
    int device_id = bytes_buff[0];

    uint16_t reg_data[(len - 1) / 2];
    memcpy(reg_data, bytes_buff + 1, len - 1);

    // 将数据写入到保持寄存器
    app_modbus_writeHoldReg(device_id, 0, (len - 1) / 2, reg_data);

    // 将device_id加入到设备id列表
    add_device_id(device_id);

    return 0;
}
int8_t receive_msg_callback(char *json)
{
    // 将json数据转换为字节数组数据
    uint8_t bytes_buff[128];
    int len = app_message_jsonToBytes(json, bytes_buff, sizeof(bytes_buff));

    // 将字节数组数据存储到下行缓冲区
    app_buffer_write(down_buffer, (char *)bytes_buff, len);

    // 将后续流程交给线程池处理
    app_pool_registerTask(write_task_func, NULL);

    return 0;
}

void remove_device_id(int device_id)
{
    // 遍历设备id列表，将device_id删除
    for (int i = 0; i < device_num; i++)
    {
        if (device_ids[i] == device_id)
        {
            device_num--;
            memmove(device_ids + i, device_ids + i + 1, (device_num - i - 1) * sizeof(int));
            break;
        }
    }
    // 显示设备id列表
    printf("device_ids: ");
    for (int i = 0; i < device_num; i++)
    {
        printf("%d ", device_ids[i]);
    }
    printf("\n");
}

// 将数据发送给上游设备
int send_task_func(void *arg)
{
    // 从上行缓冲区读取字节数组
    uint8_t bytes_buff[128];
    int len = app_buffer_read(up_buffer, (char *)bytes_buff, sizeof(bytes_buff));

    // 将字节数组数据转换为json数据
    char *json = app_message_bytesToJson(bytes_buff, len);

    // 通过mqtt将json数据发送给上游设备
    app_mqtt_send(json);

    // 如果设备已停止运行，从设备id列表删除
    int device_id = bytes_buff[0];
    uint8_t status;
    memcpy(&status, bytes_buff + 5, 1);
    if (status == 0)
    {
        remove_device_id(device_id);
    }
    free(json);
    return 0;
}

// 从指定id的设备中读取数据
int read_device_data(int device_id)
{
    // 读取多个输入寄存器数据
    uint16_t reg_data_buff[3];
    app_modbus_readInputReg(device_id, 0, 3, reg_data_buff);

    // 将数据转换为字节数组数据
    uint8_t bytes_buff[7];
    bytes_buff[0] = device_id;
    memcpy(bytes_buff + 1, reg_data_buff, 6);

    // 将字节数组数据存储到上行缓冲区
    app_buffer_write(up_buffer, (char *)bytes_buff, sizeof(bytes_buff));

    // 将后续流程交给线程池处理
    app_pool_registerTask(send_task_func, NULL);

    return 0;
}

// 不断轮询下游设备的消息
void *read_thread_func(void *arg)
{
    while (1)
    {
        for (int i = 0; i < device_num; i++)
        {
            int device_id = device_ids[i];
            read_device_data(device_id);
            usleep(5000);
        }
        sleep(1); // 休眠1秒
    }
}

void app_device_start(void)
{
    // 注册接收消息的回调函数
    app_mqtt_registerCallback(receive_msg_callback);

    // 创建一个线程不断得轮询下游设备的消息
    pthread_create(&read_thread, NULL, read_thread_func, NULL);
}

void app_device_free(void)
{
    app_mqtt_close();
    app_pool_free();
    app_modbus_free();
    app_buffer_destroy(down_buffer);
    app_buffer_destroy(up_buffer);
    pthread_cancel(read_thread);
    pthread_join(read_thread, NULL);
}
