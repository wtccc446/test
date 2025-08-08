#ifndef __HDC3021_H
#define __HDC3021_H

#include <stdint.h>
//#include "i2c_driver.h"
//#include "messageout.h"
/*==================================================================================================
 *  宏定义 (MACROS)
 *================================================================================================*/
 
// HDC3021 I2C 7位地址
#define HDC3021_I2C_ADDR_GND_GND    0x44  // ADDR=GND, ADDR1=GND
#define HDC3021_I2C_ADDR_VDD_GND    0x45  // ADDR=VDD, ADDR1=GND
#define HDC3021_I2C_ADDR_GND_VDD    0x46  // ADDR=GND, ADDR1=VDD
#define HDC3021_I2C_ADDR_VDD_VDD    0x47  // ADDR=VDD, ADDR1=VDD


// ---  HDC3021 命令  ---
#define HDC3021_CMD_SOFT_RESET      {0x30, 0xA2} // 软件复位
#define HDC3021_CMD_READ_MFG_ID     {0x37, 0x81} // 读取制造商ID

// -- 单次测量 (Trigger-On-Demand) --
#define HDC3021_CMD_MEASURE_LPM0        {0x24, 0x00} // 精度最高 (12.5ms)
#define HDC3021_CMD_MEASURE_LPM1        {0x24, 0x0B} // 精度较高 (7.5ms)
#define HDC3021_CMD_MEASURE_LPM2        {0x24, 0x16} // 精度较低 (5.0ms)
#define HDC3021_CMD_MEASURE_LPM3        {0x24, 0xFF} // 功耗最低 (3.7ms)


// -- 自动测量 (Auto Measurement) --
// 示例：每秒1次，高精度(LPM1, 7.5ms conversion)
#define HDC3021_CMD_AUTO_START_1HZ_LPM1  	{0x21, 0x26} 
#define HDC3021_CMD_AUTO_READ_DATA  			{0xE0, 0x00} // 读取自动测量的结果
#define HDC3021_CMD_AUTO_STOP       			{0x30, 0x93} // 退出自动测量模式

// -- 加热器 (Heater) --
#define HDC3021_CMD_HEATER_ENABLE       {0x30, 0x6D}
#define HDC3021_CMD_HEATER_DISABLE      {0x30, 0x66}


#define HDC3021_CMD_SET_LOW_ALERT   {0x61, 0x00} // ALERT低阈值
#define HDC3021_CMD_SET_HIGH_ALERT  {0x61, 0x1D}

// 测量等待时间 (ms) - 对应不同的测量模式
#define HDC3021_MEAS_DELAY_LPM0         15
#define HDC3021_MEAS_DELAY_LPM1         10
#define HDC3021_MEAS_DELAY_LPM2         7
#define HDC3021_MEAS_DELAY_LPM3         5

/*==================================================================================================
 *  结构体和枚举 (STRUCTURES & ENUMS)
 *================================================================================================*/
 
// 函数返回值枚举
typedef enum {
    HDC3021_OK = 0,
    HDC3021_ERR_I2C,
    HDC3021_ERR_CRC,
    HDC3021_ERR_ID_MISMATCH,
    HDC3021_ERR_DATA_NOT_READY
} hdc3021_status_t;

/*==================================================================================================
 *  函数原型 (FUNCTION PROTOTYPES)
 *================================================================================================*/
hdc3021_status_t HDC3021_Init(uint8_t i2c_addr_7bit);
hdc3021_status_t HDC3021_Read_ID(uint16_t *device_id);
hdc3021_status_t HDC3021_Temp_Humi_Read(float *temp, float *humi);

// -- 自动测量功能 --
hdc3021_status_t HDC3021_Start_Auto_Measure(void);
hdc3021_status_t HDC3021_Stop_Auto_Measure(void);
hdc3021_status_t HDC3021_Read_Data_From_Auto(float *temp, float *humi);


#endif /* __HDC3021_H */

