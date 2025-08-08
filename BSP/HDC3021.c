#include "hdc3021.h"

#include "i2c_driver.h"
#include "messageout.h"
#include "delay.h" 
#include "stdio.h"
/*==================================================================================================
 *  私有变量 (PRIVATE VARIABLES)
 *================================================================================================*/

// 将I2C地址定义为静态变量，由Init函数初始化
static uint8_t hdc3021_device_addr_8bit;

/*==================================================================================================
 *  私有函数 (PRIVATE FUNCTIONS)
 *================================================================================================*/


/**
 * @brief  CRC-8 计算
 * @param  data: 计算数据
 * @param  len: 数据长度
 * @retval 返回生成的crc
 */
static uint8_t HDC3021_CalculateCRC(const uint8_t* data, uint8_t len)
{
    uint8_t crc = 0xFF;
    const uint8_t poly = 0x31;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 8; j > 0; j--) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ poly;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}


/**
 * @brief  CRC-8 校验
 * @param  data: 待校验数据
 * @param  len: 数据长度
 * @param  checksum: 从设备接收到的CRC值
 * @retval 0 for CRC OK, 1 for CRC Fail.
 */
static uint8_t HDC3021_CheckCRC(uint8_t* data, uint8_t len, uint8_t checksum)
{
    uint8_t crc = 0xFF; // 初始值
    const uint8_t poly = 0x31; // 多项式 x^8 + x^5 + x^4 + 1
    
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 8; j > 0; j--) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ poly;
            } else {
                crc = crc << 1;
            }
        }
    }
    
    return (crc == checksum) ? 0 : 1;
}


/**
* @brief  向HDC3021发送一个命令 (2字节),不带数据
 * @param  cmd: 指向2字节命令数组的指针
 * @retval 0 for success, non-zero for failure.
 */
static hdc3021_status_t hdc3021_I2C_Write_Cmd(const uint8_t* cmd)
{
    // 将2字节命令当作“寄存器地址”发送，数据长度为0
    if (I2C1_Write_Operation(hdc3021_device_addr_8bit, 2, 0, (uint8_t*)cmd, NULL) != 0) {
        return HDC3021_ERR_I2C;
    }
    return HDC3021_OK;
}

/**
 * @brief  向HDC3021发送命令、配置数据和CRC
 */
//static hdc3021_status_t write_cmd_with_data(const uint8_t* cmd, const uint8_t* config_data)
//{
//    uint8_t data_and_crc[3];//2字节数据 + 1字节CRC
//    data_and_crc[0] = config_data[0];
//    data_and_crc[1] = config_data[1];
//    data_and_crc[2] = HDC3021_CalculateCRC(config_data, 2);

//    if (I2C1_Write_Operation(hdc3021_device_addr_8bit, 2, 3, (uint8_t*)cmd, data_and_crc) != 0) 
//		{
//        return HDC3021_ERR_I2C;
//    }
//    return HDC3021_OK;
//}


/**
 * @brief  从HDC3021读取n个数据
 * @param  data: 存储读取数据的缓冲区
 * @param  len: 要读取的字节数
 * @retval 0 for success, non-zero for failure.
 */
static hdc3021_status_t hdc3021_I2C_Read_Data(uint8_t* data, uint8_t len)
{
    if (I2C1_Read_Operation(hdc3021_device_addr_8bit, 0, len, NULL, data) != 0) {
        return HDC3021_ERR_I2C;
    }
    return HDC3021_OK;
}


/**
 * @brief  延时函数
 */
static void hdc3021_Delay(uint32_t time_ms)
{
    delay_ms(time_ms);
}


/**
 * @brief  HDC3021初始化
 * @retval 0: 成功, 1: 失败
 */
hdc3021_status_t HDC3021_Init(uint8_t i2c_addr_7bit)
{
    hdc3021_device_addr_8bit = (i2c_addr_7bit << 1);

    const uint8_t cmd[] = HDC3021_CMD_SOFT_RESET;
    if (hdc3021_I2C_Write_Cmd(cmd) != HDC3021_OK) {
        return HDC3021_ERR_I2C;
    }
    hdc3021_Delay(2); // 等待复位完成
    return HDC3021_OK;
}



/**
 * @brief  读取HDC3021的设备ID
 * @param  device_id: 用于存储16位设备ID的指针
 * @retval 0: 成功, 1: I2C通信失败, 2: CRC校验失败, 3: ID不匹配
 */
hdc3021_status_t HDC3021_Read_ID(uint16_t *device_id)
{
    const uint8_t cmd[] = HDC3021_CMD_READ_MFG_ID;
    uint8_t rx_buf[3]; // 2字节ID + 1字节CRC
    
    // 1. 发送读取ID的命令
		if (hdc3021_I2C_Write_Cmd(cmd) != HDC3021_OK) {
        return HDC3021_ERR_I2C;
    }
    
    // 2. 读取返回的3个字节
    if (hdc3021_I2C_Read_Data(rx_buf, 3) != HDC3021_OK) {
        return HDC3021_ERR_I2C;
    }
    // 3. CRC校验
    if (HDC3021_CalculateCRC(rx_buf, 2) != rx_buf[2]) {
        return HDC3021_ERR_CRC;
    }
    
    // 4. 组合ID并检查
    *device_id = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];
    printf("device_id = %x\r\n",*device_id);
    if (*device_id != 0x3000) {
        return HDC3021_ERR_ID_MISMATCH;
    }
    
    return HDC3021_OK;
}

/**
 * @brief  读取温度和湿度
 * @param  temp: 存储温度值的指针
 * @param  humi: 存储湿度值的指针
 * @retval 0: 成功, 1: I2C通信失败, 2: CRC校验失败
 */
hdc3021_status_t HDC3021_Temp_Humi_Read(float *temp, float *humi)
{
    const uint8_t cmd[] = HDC3021_CMD_MEASURE_LPM0; // 使用最高精度模式
    uint8_t rx_buffer[6]; // 2字节Temp+1CRC, 2字节Humi+1CRC
    uint16_t raw_temp, raw_humi;

    // 1. 发送触发测量命令
    if (hdc3021_I2C_Write_Cmd(cmd) != HDC3021_OK) {
        return HDC3021_ERR_I2C;
    }

    // 2. 等待测量完成
    hdc3021_Delay(HDC3021_MEAS_DELAY_LPM0);

    // 3. 读取6字节数据
    if (hdc3021_I2C_Read_Data(rx_buffer, 6) != HDC3021_OK) {
        return HDC3021_ERR_I2C;
    }

    // 4. CRC校验温度数据 (前3个字节)
    if (HDC3021_CheckCRC(&rx_buffer[0], 2, rx_buffer[2]) != 0) {
        return HDC3021_ERR_CRC;
    }

    // 5. CRC校验湿度数据 (后3个字节)
    if (HDC3021_CheckCRC(&rx_buffer[3], 2, rx_buffer[5]) != 0) {
        return HDC3021_ERR_CRC;
    }

    // 6. 组合原始数据
    raw_temp = ((uint16_t)rx_buffer[0] << 8) | rx_buffer[1];
    raw_humi = ((uint16_t)rx_buffer[3] << 8) | rx_buffer[4];

    // 7. 根据数据手册公式进行转换
    // T(°C) = 175 * (RAW_T / (2^16 - 1)) - 45
    *temp = 175.0f * (float)raw_temp / 65535.0f - 45.0f;
    // RH(%) = 100 * (RAW_RH / (2^16 - 1))
    *humi = 100.0f * (float)raw_humi / 65535.0f;

    return HDC3021_OK;
}

/* --- 自动测量API实现 --- */

/**
 * @brief  启动HDC3021的自动测量模式
 * @note   本例程配置为每秒1次，高精度(LPM1)。
 * @retval 0: 成功, 1: 失败
 */
hdc3021_status_t  HDC3021_Start_Auto_Measure(void)
{
    const uint8_t cmd[] = HDC3021_CMD_AUTO_START_1HZ_LPM1;
    return hdc3021_I2C_Write_Cmd(cmd);
}

/**
 * @brief  停止HDC3021的自动测量模式
 * @retval 0: 成功, 1: 失败
 */
hdc3021_status_t  HDC3021_Stop_Auto_Measure(void)
{
    const uint8_t cmd[] = HDC3021_CMD_AUTO_STOP;
    return hdc3021_I2C_Write_Cmd(cmd);
}

/**
 * @brief  在自动测量模式下，读取最新的温湿度数据
 * @param  temp: 存储温度值的指针
 * @param  humi: 存储湿度值的指针
 * @retval 0: 成功, 1: I2C通信失败, 2: CRC校验失败, 3: 数据未就绪
 */
hdc3021_status_t  HDC3021_Read_Data_From_Auto(float *temp, float *humi)
{
    const uint8_t cmd[] = HDC3021_CMD_AUTO_READ_DATA;
    uint8_t rx_buf[6];
    uint16_t raw_temp, raw_humi;

		// 手册图8-18表明，读取自动测量结果是“写命令->重复启动->读数据”的复合操作
    // 我们的底层 I2C1_Read_Operation 恰好能模拟这个
    if (I2C1_Read_Operation(hdc3021_device_addr_8bit, 2, 6, (uint8_t*)cmd, rx_buf) != 0) {
        return HDC3021_ERR_I2C;
    }
    
    if (rx_buf[0] == 0xFF && rx_buf[1] == 0xFF) {
        return HDC3021_ERR_DATA_NOT_READY;
    }
    
    if (HDC3021_CalculateCRC(&rx_buf[0], 2) != rx_buf[2]) return HDC3021_ERR_CRC;
    if (HDC3021_CalculateCRC(&rx_buf[3], 2) != rx_buf[5]) return HDC3021_ERR_CRC;
    
    raw_temp = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];
    raw_humi = ((uint16_t)rx_buf[3] << 8) | rx_buf[4];

    *temp = 175.0f * (float)raw_temp / 65535.0f - 45.0f;
    *humi = 100.0f * (float)raw_humi / 65535.0f;

    return HDC3021_OK;
}


hdc3021_status_t HDC3021_HeaterEnable(void)
{
    const uint8_t cmd[] = HDC3021_CMD_HEATER_ENABLE;
    return hdc3021_I2C_Write_Cmd(cmd);
}

hdc3021_status_t HDC3021_HeaterDisable(void)
{
    const uint8_t cmd[] = HDC3021_CMD_HEATER_DISABLE;
    return hdc3021_I2C_Write_Cmd(cmd);
}




