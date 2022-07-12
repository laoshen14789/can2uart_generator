#include "main.h"
#include "IIC.h"
#include "NCP5623.h"
// #include "i2c.h"
// extern I2C_HandleTypeDef hi2c1;

int ret  = -1;
void NCP5623_write(uint8_t reg, uint8_t data)
{
    uint8_t msg[1] = { 0x00 };
    // scale to 0 to 0x1F
    data = (data * 255U) / 0x1FU;
    msg[0] = ((reg & 0xe0) | (data & 0x1f));
    // ret = HAL_I2C_Master_Transmit(&hi2c1,NCP5623_DEFAULT_ADDR,msg,1,100);
    Device_WriteData(NCP5623_LED_I2C_ADDR, msg,1);
}

void NCP5623_write_pwm(uint8_t _rgb[3])
{
    uint8_t reg = NCP5623_LED_PWM0;
    for (uint8_t i = 0; i < 3; i++)
    {
        NCP5623_write(reg + i * 0x20, _rgb[i]);
    }
}

