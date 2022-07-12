#include "generator.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "status.h"
generator_data_s generator_data;
extern generator_info_s generator_info;
uint8_t data[70];

static uint32_t endianSwap(uint32_t data, uint32_t length)
{
	uint32_t next;
	uint32_t i = 0;
	while (length-- > 0)
	{
		*(((uint8_t *)&next) + i++) = *(((uint8_t *)&data) + length);
	}
	return next;
}

void send_generator()
{
	uint16_t checksum = 0;
	memcpy(data, &generator_data, sizeof(generator_data_s));
	uint8_t *checksum_buffer = &data[2];
	for (uint8_t i = 0; i < 5; i++)
	{
		checksum += (checksum_buffer[2 * i]);
	}
//	memcpy(&generator_data.checksum,checksum,2);
	generator_data.checksum = checksum;
	memcpy(data, &generator_data, sizeof(generator_data_s));
	HAL_UART_Transmit(&huart1, data, sizeof(data), 50);

	set_status(CAN_RX_STATUS, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
//int cnt = 0;
void update_generator_data()
{
//	if(cnt >= 30)
//	{
//	cnt =0;
//	}
//	cnt ++;
//	memset(&generator_data,0,sizeof(generator_data));
	generator_data.headermagic1 = 0xAA;
	generator_data.headermagic2 = 0x55;
	generator_data.version = 0x00;
//	generator_data.version = endianSwap(generator_info.generator0_s.ExtenderVersion * 1000, 2);
	generator_data.rpm = endianSwap(generator_info.generator1_s.EngineSpeed, 2);
	generator_data.throttle = endianSwap(generator_info.generator1_s.ThrottlePosition, 2);
	generator_data.output_current = endianSwap(generator_info.generator2_s.OutputCurrent * 20, 2);
	generator_data.output_voltage = endianSwap(generator_info.generator2_s.OutputVoltage * 20, 2);
	
	if(generator_info.generator1_s.EngineState == 0)	////run 2, off 0
	{
		generator_data.mode = 4;		//run 1, off 4
	}
	else if(generator_info.generator1_s.EngineState == 2)
	{
		generator_data.mode = 1;
	}

//	generator_data.output_voltage = endianSwap(cnt*2 * 100, 2);
//	generator_data.output_current = endianSwap(2 * 100, 2);
//	generator_data.rpm = endianSwap(cnt*10, 2);
//	generator_data.output_voltage = 0xB01D;
	generator_data.footermagic1 = 0x55;
	generator_data.footermagic2 = 0xAA;
	send_generator();
	
}


int get_oil_level()
{
	uint8_t oilLevel = 0;
	float FuelPosition = 0;
	int transOut = 0;
	oilLevel = generator_info.generator1_s.FuelPosition;
	FuelPosition = oilLevel/200.0;
	transOut = 1000+(1000*FuelPosition);
	return transOut;
}
