#include "IIC.h"
#include "main.h"

void delay_us(int us)
{
	int i = 0;
	for (i = 0; i < us * 50; i++)
	{
		__NOP;
	}
}
//产生IIC起始信号
void IIC_Start(void)
{

	I2C_SDA_1();
	I2C_SCL_1();
	delay_us(4);
	I2C_SDA_0(); //START:when CLK is high,DATA change form high to low
	delay_us(4);
	I2C_SCL_0(); //钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{

	I2C_SCL_0();
	I2C_SDA_0(); //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	I2C_SCL_1();
	I2C_SDA_1(); //发送I2C总线结束信号
	delay_us(4);
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;
	I2C_SCL_0(); //拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if (((txd & 0x80) >> 7))
		{
			I2C_SDA_1();
			txd <<= 1;
			delay_us(2); //对TEA5767这三个延时都是必须的
			I2C_SCL_1();
			delay_us(2);
			I2C_SCL_0();
			delay_us(2);
		}
		else
		{
			I2C_SDA_0();
			txd <<= 1;
			delay_us(2); //对TEA5767这三个延时都是必须的
			I2C_SCL_1();
			delay_us(2);
			I2C_SCL_0();
			delay_us(2);
		}
	}
}

void Device_WriteData(uint8_t addr, uint8_t *data, int size)
{
	int i = 0;
	// IIC_Start();
	IIC_Send_Byte(addr);
	for (i = 0; i < size; i++)
	{
		IIC_Send_Byte(data[i]);
	}
	// IIC_Stop();
}
