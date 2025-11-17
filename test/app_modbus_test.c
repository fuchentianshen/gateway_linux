#include "app_modbus.h"
#include <string.h>
int main(int argc, char const *argv[])
{
    // 初始化modbus
    app_modbus_init("/dev/pts/5");

    // 写线圈
    uint8_t coil_data = 0x01;
    app_modbus_writeCoils(1, 2, 1, &coil_data);

    // 读线圈
    uint8_t coil_data_buff;
    app_modbus_readCoils(1, 2, 1, &coil_data_buff);
    log_debug("读线圈: %d", coil_data_buff);

    // 写保持寄存器
    // target_angle 3600 max_speed 60
    float target_angle = 3600;
    float max_speed = 60;
    uint16_t reg_data[4];
    memcpy(reg_data, &target_angle, sizeof(float));
    memcpy(reg_data + 2, &max_speed, sizeof(float));
    app_modbus_writeHoldReg(1, 0, 4, reg_data);

    // 读输入寄存器
    uint16_t reg_data_buff[3];
    app_modbus_readInputReg(1, 0, 3, reg_data_buff);
    float cur_angle;
    uint8_t motor_status;
    memcpy(&cur_angle, reg_data_buff, 4);
    // motor_status = (uint8_t)reg_data_buff[2];
    memcpy(&motor_status, reg_data_buff + 2, 1);

    log_debug("读取角度：%f, 电机状态：%d", cur_angle, motor_status);

    return 0;
}
