#include "hdc3021.h"
#include "delay.h" 
#include "stdio.h"

// 定义HDC3021的8位I2C地址
// (0x44 << 1) = 0x88
const uint8_t hdc3021_id = (HDC3021_I2C_ADDR_7BIT_1 << 1);

/**
 * @brief  向HDC3021发送一个命令 (2字节)
 * @param  cmd: 指向2字节命令数组的指针
 * @retval 0 for success, non-zero for failure.
 */
static uint8_t hdc3021_I2C_Write_Cmd(const uint8_t* cmd)
{
    // 调用底层I2C写函数
    // 关键: regadd_len = 0, 表示不发送寄存器地址
    // 将2字节命令作为数据发送
    return I2C1_Write_Operation(hdc3021_id, 0, 2, NULL, (uint8_t*)cmd);
}

/**
 * @brief  从HDC3021读取数据
 * @param  data: 存储读取数据的缓冲区
 * @param  len: 要读取的字节数
 * @retval 0 for success, non-zero for failure.
 */
static uint8_t hdc3021_I2C_Read_Data(uint8_t* data, uint8_t len)
{
    // 关键: regadd_len = 0, 表示在读取前不发送任何寄存器地址
    return I2C1_Read_Operation(hdc3021_id, 0, len, NULL, data);
}

/**
 * @brief  延时函数
 */
static void hdc3021_Delay(uint32_t time_ms)
{
    delay_ms(time_ms);
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
 * @brief  初始化HDC3021 (软复位)
 * @retval 0: 成功, 1: 失败
 */
static uint8_t HDC3021_SOFTRESET(void)
{
    const uint8_t cmd[] = HDC3021_CMD_SOFT_RESET;
    
    if (hdc3021_I2C_Write_Cmd(cmd) != 0) {
        return 1; // 发送复位命令失败
    }
     //复位后等待1.3ms (t_RESET_RDY)
    hdc3021_Delay(2); // 等待复位完成
    return 0;
}
/**
 * @brief  初始化HDC3021 (软复位)
 * @retval 0: 成功, 1: 失败
 */
uint8_t HDC3021_Init(void)
{
	if(!HDC3021_SOFTRESET())
	{
		return 1;
	}
	
	return 0;
}

/**
 * @brief  读取HDC3021的设备ID
 * @param  device_id: 用于存储16位设备ID的指针
 * @retval 0: 成功, 1: I2C通信失败, 2: CRC校验失败, 3: ID不匹配
 */
uint8_t HDC3021_Read_ID(uint16_t *device_id)
{
    const uint8_t cmd[] = HDC3021_CMD_READ_MFG_ID;
    uint8_t rx_buffer[3]; // 2字节ID + 1字节CRC
    
    // 1. 发送读取ID的命令
    if (hdc3021_I2C_Write_Cmd(cmd) != 0) {
        return 1;
    }
    
    // 2. 读取返回的3个字节
    if (hdc3021_I2C_Read_Data(rx_buffer, 3) != 0) {
        return 1;
    }
    
    // 3. CRC校验
    if (HDC3021_CheckCRC(rx_buffer, 2, rx_buffer[2]) != 0) {
        return 2;
    }
    
    // 4. 组合ID并检查
    *device_id = ((uint16_t)rx_buffer[0] << 8) | rx_buffer[1];
    printf("device_id = %x\r\n",*device_id);
    if (*device_id != 0x0C1D) {
        return 3; // HDC302x 系列的ID固定为 0x0C1D
    }
    
    return 0;
}

/**
 * @brief  读取温度和湿度
 * @param  temp: 存储温度值的指针
 * @param  humi: 存储湿度值的指针
 * @retval 0: 成功, 1: I2C通信失败, 2: CRC校验失败
 */
uint8_t HDC3021_Temp_Humi_Read(float *temp, float *humi)
{
    const uint8_t cmd[] = HDC3021_CMD_MEASURE_HIGH;
    uint8_t rx_buffer[6]; // 2字节Temp+1CRC, 2字节Humi+1CRC
    uint16_t raw_temp, raw_humi;

    // 1. 发送触发测量命令
    if (hdc3021_I2C_Write_Cmd(cmd) != 0) {
        return 1;
    }

    // 2. 等待测量完成
    hdc3021_Delay(HDC3021_MEAS_DURATION_MS);

    // 3. 读取6字节数据
    if (hdc3021_I2C_Read_Data(rx_buffer, 6) != 0) {
        return 1;
    }

    // 4. CRC校验温度数据 (前3个字节)
    if (HDC3021_CheckCRC(&rx_buffer[0], 2, rx_buffer[2]) != 0) {
        return 2;
    }

    // 5. CRC校验湿度数据 (后3个字节)
    if (HDC3021_CheckCRC(&rx_buffer[3], 2, rx_buffer[5]) != 0) {
        return 2;
    }

    // 6. 组合原始数据
    raw_temp = ((uint16_t)rx_buffer[0] << 8) | rx_buffer[1];
    raw_humi = ((uint16_t)rx_buffer[3] << 8) | rx_buffer[4];

    // 7. 根据数据手册公式进行转换
    // T(°C) = 175 * (RAW_T / (2^16 - 1)) - 45
    *temp = 175.0f * (float)raw_temp / 65535.0f - 45.0f;
    // RH(%) = 100 * (RAW_RH / (2^16 - 1))
    *humi = 100.0f * (float)raw_humi / 65535.0f;

    return 0;
}


/* --- 自动测量API实现 --- */

/**
 * @brief  启动HDC3021的自动测量模式
 * @note   本例程配置为每秒1次，高精度(LPM1)。
 * @retval 0: 成功, 1: 失败
 */
uint8_t HDC3021_Start_Auto_Measure(void)
{
    const uint8_t cmd[] = HDC3021_CMD_AUTO_START_1HZ;
    return hdc3021_I2C_Write_Cmd(cmd);
}

/**
 * @brief  停止HDC3021的自动测量模式
 * @retval 0: 成功, 1: 失败
 */
uint8_t HDC3021_Stop_Auto_Measure(void)
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
uint8_t HDC3021_Read_Data_From_Auto(float *temp, float *humi)
{
    const uint8_t cmd[] = HDC3021_CMD_AUTO_READ_DATA;
    uint8_t rx_buffer[6];
    uint16_t raw_temp, raw_humi;

    // 1. 发送读取命令
    if (hdc3021_I2C_Write_Cmd(cmd) != 0) {
        return 1;
    }

    // 2. 读取6字节数据 (注意：这里不需要延时，因为我们是去取现成的数据)
    if (hdc3021_I2C_Read_Data(rx_buffer, 6) != 0) {
        return 1;
    }

    // 检查数据是否有效。根据手册，如果数据还未准备好，
    // 读取到的温度会是 +130°C，湿度是 +100%RH。
    // 这对应于原始数据接近 0xFFFF。我们检查温度值即可。
    if (rx_buffer[0] == 0xFF && rx_buffer[1] == 0xFF) {
        return 3; // 返回错误码，表示数据未就绪
    }
    
    // 3. CRC校验
    if (HDC3021_CheckCRC(&rx_buffer[0], 2, rx_buffer[2]) != 0) return 2;
    if (HDC3021_CheckCRC(&rx_buffer[3], 2, rx_buffer[5]) != 0) return 2;

    // 4. 转换数据
    raw_temp = ((uint16_t)rx_buffer[0] << 8) | rx_buffer[1];
    raw_humi = ((uint16_t)rx_buffer[3] << 8) | rx_buffer[4];

    *temp = 175.0f * (float)raw_temp / 65535.0f - 45.0f;
    *humi = 100.0f * (float)raw_humi / 65535.0f;

    return 0;
}


//TMP117 GET TMP
//void TMP117_Read_Cmd(uint8_t *buff)
//{
//	float v;
//	if(buff[0]=='T' && buff[5]=='7' && buff[13]=='P')
//	{
//		v = TMP117_Get_Temp();
//		MessageSend("Temp value: %f\r\n",v);
//	}
//}






