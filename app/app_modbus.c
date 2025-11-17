#include "app_modbus.h"

static modbus_t *modbus = NULL;

int app_modbus_init(char *filename)
{
    // 创建modbus对象
    modbus = modbus_new_rtu(filename, 115200, 'N', 8, 1);

    // 设置1：显示调试日志
    modbus_set_debug(modbus, TRUE);
    // 设置2：设置RTS,指定自动控制收发方向
    modbus_rtu_set_rts(modbus, MODBUS_RTU_RTS_NONE);
    // 设置3：设置超时时间
    modbus_set_response_timeout(modbus, 1, 0);

    // 创建连接
    int result = modbus_connect(modbus);
    if (result == -1)
    {
        log_error("连接modbus失败:%s", modbus_strerror(errno));
        modbus_free(modbus);
        return -1;
    }
    log_debug("连接modbus成功");
    return 0;
}

void app_modbus_free(void)
{
    // 关闭连接
    modbus_close(modbus);
    // 释放资源
    modbus_free(modbus);
}

int app_modbus_writeHoldReg(int device_id, int addr, int reg_num, uint16_t *reg_data)
{
    // 刷新清理缓存
    modbus_flush(modbus);
    // 设置从设备id
    modbus_set_slave(modbus, device_id);
    // 写入寄存器
    if (modbus_write_registers(modbus, addr, reg_num, reg_data) == -1)
    {
        // 等待5ms后再次写入
        usleep(5000);
        if (modbus_write_registers(modbus, addr, reg_num, reg_data) == -1)
        {
            log_error("modbus写入保持寄存器失败:%s", modbus_strerror(errno));
            return -1;
        }
    }
    log_debug("modbus写入保持寄存器成功");
    return 0;
}

int app_modbus_readInputReg(int device_id, int addr, int reg_num, uint16_t *reg_data_buff)
{
    // 刷新清理缓存
    modbus_flush(modbus);
    // 设置从设备id
    modbus_set_slave(modbus, device_id);
    // 读取输入寄存器
    if (modbus_read_input_registers(modbus, addr, reg_num, reg_data_buff) == -1)
    {
        // 等待5ms后再次读取
        usleep(5000);
        if (modbus_read_input_registers(modbus, addr, reg_num, reg_data_buff) == -1)
        {
            log_error("modbus读取输入寄存器失败:%s", modbus_strerror(errno));
            return -1;
        }
    }

    return 0;
}

// 写入线圈
int app_modbus_writeCoils(int device_id, int addr, int coil_num, uint8_t *coil_data)
{
    // 刷新清理缓存
    modbus_flush(modbus);
    // 设置从设备id
    modbus_set_slave(modbus, device_id);
    // 写入线圈
    if (modbus_write_bits(modbus, addr, coil_num, coil_data) == -1)
    {
        // 等待5ms后再次写入
        usleep(5000);
        if (modbus_write_bits(modbus, addr, coil_num, coil_data) == -1)
        {
            log_error("modbus写入线圈失败:%s", modbus_strerror(errno));
            return -1;
        }
    }
    log_debug("modbus写入线圈成功");
    return 0;
}

int app_modbus_readCoils(int device_id, int addr, int coil_num, uint8_t *coil_data_buff)
{
    // 刷新清理缓存
    modbus_flush(modbus);
    // 设置从设备id
    modbus_set_slave(modbus, device_id);
    // 读取线圈
    if (modbus_read_bits(modbus, addr, coil_num, coil_data_buff) == -1)
    {
        // 等待5ms后再次读取
        usleep(5000);
        if (modbus_read_bits(modbus, addr, coil_num, coil_data_buff) == -1)
        {
            log_error("modbus读取线圈失败:%s", modbus_strerror(errno));
            return -1;
        }
    }

    return 0;
}
