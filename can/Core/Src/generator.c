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
	const uint8_t *checksum_buffer = &data[2];
	for (uint8_t i = 0; i < 5; i++)
	{
		checksum += (checksum_buffer[2 * i]) + checksum;
	}
	generator_data.checksum = checksum;
	memcpy(data, &generator_data, sizeof(generator_data_s));
	HAL_UART_Transmit(&huart1, data, sizeof(data), 10);
	set_status(CAN_RX_STATUS, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void update_generator_data()
{
	generator_data.headermagic1 = 0xAA;
	generator_data.headermagic2 = 0x55;
	generator_data.version = endianSwap(generator_info.generator0_s.ExtenderVersion * 1000, 2);
	generator_data.rpm = endianSwap(generator_info.generator1_s.EngineSpeed, 2);
	generator_data.throttle = endianSwap(generator_info.generator1_s.ThrottlePosition, 2);
	generator_data.output_current = endianSwap(generator_info.generator2_s.OutputCurrent * 100, 2);
	generator_data.output_voltage = endianSwap(generator_info.generator2_s.OutputVoltage * 100, 2);
	//	generator_data.output_voltage = 0xB01D;
	generator_data.footermagic1 = 0x55;
	generator_data.footermagic2 = 0xAA;
	send_generator();
}
