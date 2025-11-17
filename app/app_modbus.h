#ifndef __APP_MODBUS_H__
#define __APP_MODBUS_H__

#define _GNU_SOURCE
#include <modbus/modbus.h>
#include "log.h"
#include "errno.h"
#include <unistd.h>

/**
 * @brief modbus初始化
 *
 * @param filename 配置文件名
 * @return int 0:成功 -1:失败
 */
int app_modbus_init(char *filename);

/**
 * @brief 释放modbus
 *
 */
void app_modbus_free(void);

/**
 * @brief 写入多个保持寄存器
 *
 * @param device_id 设备id
 * @param addr 寄存器地址
 * @param reg_num 寄存器数量
 * @param reg_data 要写入的寄存器数据
 * @return int 0:成功 -1:失败
 */
int app_modbus_writeHoldReg(int device_id, int addr, int reg_num, uint16_t *reg_data);

/**
 * @brief 读取多个输入寄存器
 * 
 * @param device_id 设备id
 * @param addr 寄存器地址
 * @param reg_num 寄存器数量
 * @param reg_data_buff 读取的寄存器数据
 * @return int 0:成功 -1:失败
 */
int app_modbus_readInputReg(int device_id, int addr, int reg_num, uint16_t *reg_data_buff);

//写多个线圈
int app_modbus_writeCoils(int device_id, int addr, int coil_num, uint8_t *coil_data);

//读取多个线圈
int app_modbus_readCoils(int device_id, int addr, int coil_num, uint8_t *coil_data_buff);

#endif /* __APP_MODBUS_H__ */
