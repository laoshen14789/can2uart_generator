
#include "stdint.h"

#define NCP5623_DEFAULT_ADDR 0x38
#define NCP5623_REG_ILED 1
#define NCP5623_REG_CHANNEL_BASE 2

#define NCP5623_LED_BRIGHT  0x1f    // full brightness
#define NCP5623_LED_MEDIUM  0x18    // medium brightness
#define NCP5623_LED_DIM     0x0f    // dim
#define NCP5623_LED_OFF     0x00    // off

#define NCP5623_LED_I2C_ADDR 0x39    // default I2C bus address
#define NCP5623_C_LED_I2C_ADDR 0x39    // default I2C bus address for the NCP5623C

#define NCP5623_LED_PWM0    0x40    // pwm0 register
#define NCP5623_LED_PWM1    0x60    // pwm1 register
#define NCP5623_LED_PWM2    0x80    // pwm2 register
#define NCP5623_LED_ENABLE  0x20    // enable register


void NCP5623_write(uint8_t reg, uint8_t data);
void NCP5623_write_pwm(uint8_t _rgb[3]);
