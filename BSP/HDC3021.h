#ifndef __HDC3021_H
#define __HDC3021_H

#include <stdint.h>
#include "i2c_driver.h"
#include "messageout.h"

// HDC3021 I2C 7位地址
#define HDC3021_I2C_ADDR_7BIT_1       0x44  //ADDR LOW    ADDR1 LOW
#define HDC3021_I2C_ADDR_7BIT_2       0x45  //ADDR HIGH   ADDR1 LOW
#define HDC3021_I2C_ADDR_7BIT_3       0x46  //ADDR LOW    ADDR1 HIGH
#define HDC3021_I2C_ADDR_7BIT_4       0x47  //ADDR HIGH   ADDR1 HIGH

// ---  HDC3021 命令  ---
#define HDC3021_CMD_SOFT_RESET      {0x30, 0xA2} // 软件复位
#define HDC3021_CMD_READ_MFG_ID     {0x37, 0x81} // 读取制造商ID
#define HDC3021_CMD_MEASURE_HIGH    {0x24, 0x00} // 高精度单次测量，最低噪声

// -- 自动测量 (Auto Measurement) --
// 示例：每秒1次，高精度(LPM1, 7.5ms conversion)
#define HDC3021_CMD_AUTO_START_1HZ  {0x21, 0x26} 
#define HDC3021_CMD_AUTO_READ_DATA  {0xE0, 0x00} // 读取自动测量的结果
#define HDC3021_CMD_AUTO_STOP       {0x30, 0x93} // 退出自动测量模式

#define HDC3021_CMD_SET_LOW_ALERT   {0x61, 0x00} // ALERT低阈值
#define HDC3021_CMD_SET_HIGH_ALERT  {0x61, 0x1D}

// 测量等待时间 (ms)
#define HDC3021_MEAS_DURATION_MS    15 // 高精度模式测量时间为 12.5ms，留些余量

uint8_t HDC3021_Init(void);
//uint8_t HDC3021_SOFTRESET(void);
//uint8_t HDC3021_Read_Manufacturer_ID(uint16_t *mfg_id);
uint8_t HDC3021_Read_ID(uint16_t *device_id);
uint8_t HDC3021_Temp_Humi_Read(float *temp, float *humi);

// -- 自动测量功能 --
uint8_t HDC3021_Start_Auto_Measure(void);
uint8_t HDC3021_Stop_Auto_Measure(void);
uint8_t HDC3021_Read_Data_From_Auto(float *temp, float *humi);


#endif /* __HDC3021_H */

