//i2c.h
#include "stm32f1xx_hal.h"

#define GPIO_PORT_I2C GPIOB // GPIO端口
#define I2C_SCL_PIN SCL_Pin // 连接到SCL时钟线的GPIO
#define I2C_SDA_PIN SDA_Pin // 连接到SDA数据线的GPIO

/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1() HAL_GPIO_WritePin(GPIOB, SCL_Pin, GPIO_PIN_SET); // SCL = 1
#define I2C_SCL_0() HAL_GPIO_WritePin(GPIOB, SCL_Pin, GPIO_PIN_RESET);   // SCL = 0

#define I2C_SDA_1() HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_SET); // SDA = 1
#define I2C_SDA_0() HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_RESET);   // SDA = 0


 
//IIC所有操作函数			 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
void Device_WriteData(uint8_t addr, uint8_t *data, int size);


